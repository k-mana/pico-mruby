// SPDX-FileCopyrightText: Copyright (c) 2021 k-mana
// SPDX-License-Identifier: MIT
#include "mruby.h"

#include "pico/time.h"

static mrb_value
mrb_time_msleep(mrb_state *mrb, mrb_value self)
{
  mrb_int msec;
  mrb_get_args(mrb, "i", &msec);
  sleep_ms(msec);
  return mrb_nil_value();
}

static mrb_value
mrb_time_usleep(mrb_state *mrb, mrb_value self)
{
  mrb_int usec;
  mrb_get_args(mrb, "i", &usec);
  sleep_us(usec);
  return mrb_nil_value();
}

void
mrb_mruby_raspberrypipico_gem_init(mrb_state *mrb)
{
  struct RClass *rasp, *time;
  rasp = mrb_define_module(mrb, "RaspberryPiPico");
  time = mrb_define_module_under(mrb, rasp, "Time");
  mrb_define_class_method(mrb, time, "msleep", mrb_time_msleep, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, time, "usleep", mrb_time_usleep, MRB_ARGS_REQ(1));
}

void
mrb_mruby_raspberrypipico_gem_final(mrb_state *mrb)
{
}
