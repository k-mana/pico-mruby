// SPDX-License-Identifier: MIT
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/time.h"

#include "mruby.h"
#include "mruby/array.h"
#include "mruby/proc.h"
#include "mruby/compile.h"
#include "mruby/dump.h"
#include "mruby/string.h"
#include "mruby/variable.h"
#include "mruby/presym.h"
#include "mruby/version.h"

#include "raspberrypipico.h"

#ifdef DISABLE_MIRB_UNDERSCORE
# define MRB_NO_MIRB_UNDERSCORE
#endif
#ifdef ENABLE_MIRB_DEBUG
# define MRB_MIRB_DEBUG TRUE
#else
# define MRB_MIRB_DEBUG FALSE
#endif
#ifdef ENABLE_MIRB_VERBOSE
# define MRB_MIRB_VERBOSE
#endif

static void
p(mrb_state *mrb, mrb_value obj, int prompt)
{
  mrb_value val;

  val = mrb_funcall_id(mrb, obj, MRB_SYM(inspect), 0);
  if (prompt) {
    if(!mrb->exc) {
      printf(" => ");
    }
    else {
      val = mrb_funcall_id(mrb, mrb_obj_value(mrb->exc), MRB_SYM(inspect), 0);
    }
  }
  if (!mrb_string_p(val)) {
    val = mrb_obj_as_string(mrb, obj);
  }
  printf("%s\n", RSTRING_PTR(val));
}

/* Guess if the user might want to enter more
 * or if they wants an evaluation of their code now */
static mrb_bool
is_code_block_open(struct mrb_parser_state *parser)
{
  mrb_bool code_block_open = FALSE;

  /* check for heredoc */
  if (parser->parsing_heredoc != NULL) return TRUE;

  /* check for unterminated string */
  if (parser->lex_strterm) return TRUE;

  /* check if parser error are available */
  if (0 < parser->nerr) {
    const char unexpected_end[] = "syntax error, unexpected $end";
    const char *message = parser->error_buffer[0].message;

    /* a parser error occur, we have to check if */
    /* we need to read one more line or if there is */
    /* a different issue which we have to show to */
    /* the user */

    if (strncmp(message, unexpected_end, sizeof(unexpected_end) - 1) == 0) {
      code_block_open = TRUE;
    }
    else if (strcmp(message, "syntax error, unexpected keyword_end") == 0) {
      code_block_open = FALSE;
    }
    else if (strcmp(message, "syntax error, unexpected tREGEXP_BEG") == 0) {
      code_block_open = FALSE;
    }
    return code_block_open;
  }

  switch (parser->lstate) {

  /* all states which need more code */

  case EXPR_BEG:
    /* beginning of a statement, */
    /* that means previous line ended */
    code_block_open = FALSE;
    break;
  case EXPR_DOT:
    /* a message dot was the last token, */
    /* there has to come more */
    code_block_open = TRUE;
    break;
  case EXPR_CLASS:
    /* a class keyword is not enough! */
    /* we need also a name of the class */
    code_block_open = TRUE;
    break;
  case EXPR_FNAME:
    /* a method name is necessary */
    code_block_open = TRUE;
    break;
  case EXPR_VALUE:
    /* if, elsif, etc. without condition */
    code_block_open = TRUE;
    break;

  /* now all the states which are closed */

  case EXPR_ARG:
    /* an argument is the last token */
    code_block_open = FALSE;
    break;

  /* all states which are unsure */

  case EXPR_CMDARG:
    break;
  case EXPR_END:
    /* an expression was ended */
    break;
  case EXPR_ENDARG:
    /* closing parenthesis */
    break;
  case EXPR_ENDFN:
    /* definition end */
    break;
  case EXPR_MID:
    /* jump keyword like break, return, ... */
    break;
  case EXPR_MAX_STATE:
    /* don't know what to do with this token */
    break;
  default:
    /* this state is unexpected! */
    break;
  }

  return code_block_open;
}

/* Print a short remark for the user */
static void
print_hint(void)
{
  printf("mirb - Embeddable Interactive Ruby Shell\n");
  printf("%s\n", MRUBY_DESCRIPTION);
  printf("%s\n", MRUBY_COPYRIGHT);
  printf("\n");
}

/* Print the command line prompt of the REPL */
static void
print_cmdline(int code_block_open)
{
  if (code_block_open) {
    printf("* ");
  }
  else {
    printf("> ");
  }
  fflush(NULL);
}

#ifdef MRB_MIRB_VERBOSE
void mrb_codedump_all(mrb_state*, struct RProc*);
#endif

static int
check_keyword(const char *buf, const char *word)
{
  const char *p = buf;
  size_t len = strlen(word);

  /* skip preceding spaces */
  while (*p && ISSPACE(*p)) {
    p++;
  }
  /* check keyword */
  if (strncmp(p, word, len) != 0) {
    return 0;
  }
  p += len;
  /* skip trailing spaces */
  while (*p) {
    if (!ISSPACE(*p)) return 0;
    p++;
  }
  return 1;
}

#ifndef MRB_NO_MIRB_UNDERSCORE
static int
decl_lv_underscore(mrb_state *mrb, mrbc_context *cxt)
{
  struct RProc *proc;
  struct mrb_parser_state *parser;

  parser = mrb_parse_string(mrb, "_=nil", cxt);
  if (parser == NULL) {
    puts("create parser state error");
    mrb_close(mrb);
    return 1;
  }

  proc = mrb_generate_code(mrb, parser);
  mrb_vm_run(mrb, proc, mrb_top_self(mrb), 0);

  mrb_parser_free(parser);
  return 0;
}
#endif

static int
RunMIRB()
{
  char ruby_code[4096] = { 0 };
  char last_code_line[1024] = { 0 };
  int last_char;
  size_t char_index;
  mrbc_context *cxt;
  struct mrb_parser_state *parser;
  mrb_state *mrb;
  mrb_value result;
  mrb_bool code_block_open = FALSE;
  int ai;
  unsigned int stack_keep = 0;

  /* new interpreter instance */
  mrb = mrb_open();
  if (mrb == NULL) {
    puts("Invalid mrb interpreter, exiting mirb");
    return 1;
  }

  mrb_mruby_raspberrypipico_gem_init(mrb);

  mrb_gv_set(mrb, mrb_intern_lit(mrb, "$DEBUG"), mrb_bool_value(MRB_MIRB_DEBUG));

  print_hint();

  cxt = mrbc_context_new(mrb);

#ifndef MRB_NO_MIRB_UNDERSCORE
  if (decl_lv_underscore(mrb, cxt))
    return 1;
#endif

  cxt->capture_errors = TRUE;
  cxt->lineno = 1;
  mrbc_filename(mrb, cxt, "(mirb)");
#ifdef MRB_MIRB_VERBOSE
  cxt->dump_result = TRUE;
#endif

  ai = mrb_gc_arena_save(mrb);

  while (TRUE) {
    print_cmdline(code_block_open);

    char_index = 0;
    while ((last_char = getchar()) != '\n') {
      if (last_char == '\r') {
        last_char = '\n';
	break;
      }
      if (last_char == EOF) break;
      if (char_index >= sizeof(last_code_line)-2) {
        puts("input string too long");
        continue;
      }
      if (isprint(last_char))
        putchar(last_char);
      if (last_char == '\b') {
        if (char_index > 0) {
	  putchar('\b');
          printf("\x1b[\x01P");
          char_index--;
        }
      }
      else {
        last_code_line[char_index++] = last_char;
      }
    }
    putchar('\n');

    if (last_char == EOF) {
      break;
    }

    last_code_line[char_index++] = '\n';
    last_code_line[char_index] = '\0';

    if (code_block_open) {
      if (strlen(ruby_code) + strlen(last_code_line) > sizeof(ruby_code)-1) {
        puts("concatenated input string too long");
        continue;
      }
      strcat(ruby_code, last_code_line);
    }
    else {
      if (check_keyword(last_code_line, "quit") || check_keyword(last_code_line, "exit")) {
        break;
      }
      strcpy(ruby_code, last_code_line);
    }

    /* parse code */
    parser = mrb_parser_new(mrb);
    if (parser == NULL) {
      puts("create parser state error");
      break;
    }
    parser->s = ruby_code;
    parser->send = ruby_code + strlen(ruby_code);
    parser->lineno = cxt->lineno;
    mrb_parser_parse(parser, cxt);
    code_block_open = is_code_block_open(parser);

    if (code_block_open) {
      /* no evaluation of code */
    }
    else {
      if (0 < parser->nwarn) {
        /* warning */
        printf("line %d: %s\n", parser->warn_buffer[0].lineno, parser->warn_buffer[0].message);
      }
      if (0 < parser->nerr) {
        /* syntax error */
        printf("line %d: %s\n", parser->error_buffer[0].lineno, parser->error_buffer[0].message);
      }
      else {
        /* generate bytecode */
        struct RProc *proc = mrb_generate_code(mrb, parser);
        if (proc == NULL) {
          mrb_parser_free(parser);
          continue;
        }

#ifdef MRB_MIRB_VERBOSE
	mrb_codedump_all(mrb, proc);
#endif
        /* adjust stack length of toplevel environment */
        if (mrb->c->cibase->u.env) {
          struct REnv *e = mrb_vm_ci_env(mrb->c->cibase);
          if (e && MRB_ENV_LEN(e) < proc->body.irep->nlocals) {
            MRB_ENV_SET_LEN(e, proc->body.irep->nlocals);
          }
        }
        /* pass a proc for evaluation */
        /* evaluate the bytecode */
        result = mrb_vm_run(mrb,
            proc,
            mrb_top_self(mrb),
            stack_keep);
        stack_keep = proc->body.irep->nlocals;
        /* did an exception occur? */
        if (mrb->exc) {
          p(mrb, mrb_obj_value(mrb->exc), 0);
          mrb->exc = 0;
        }
        else {
          /* no */
          if (!mrb_respond_to(mrb, result, MRB_SYM(inspect))) {
            result = mrb_any_to_s(mrb, result);
          }
          p(mrb, result, 1);
#ifndef MRB_NO_MIRB_UNDERSCORE
          *(mrb->c->ci->stack + 1) = result;
#endif
        }
      }
      ruby_code[0] = '\0';
      last_code_line[0] = '\0';
      mrb_gc_arena_restore(mrb, ai);
    }
    mrb_parser_free(parser);
    cxt->lineno++;
  }
  mrbc_context_free(mrb, cxt);
  mrb_close(mrb);

  return 0;
}

int
main()
{
  stdio_init_all();
  sleep_ms(1000);

  while (1) {
    RunMIRB();
  }

  return 0;
}
