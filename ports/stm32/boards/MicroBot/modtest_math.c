#include "stdint.h"
#include "stdio.h"

#include "py/obj.h"
#include "py/runtime.h"

#include "modtest_math.h"

typedef struct _math_obj_t
{
    mp_obj_base_t base;
    uint16_t value1;
    uint16_t value2;
}math_obj_t;


STATIC mp_obj_t math_nothing()
{
    printf("this is the function in class type and no parameter! \n");
    return mp_const_none;
}

mp_obj_t math_add(mp_obj_t self_in, mp_obj_t data)
{
    math_obj_t *self = MP_OBJ_TO_PTR(self_in);
    self -> value1 = 100;
    self -> value2 = mp_obj_get_int(data);
    printf("100 + %d = \n", self->value2);
    return mp_obj_new_int(self->value1 + self->value2);
}

STATIC const MP_DEFINE_CONST_FUN_OBJ_0(math_nothing_obj, math_nothing);
STATIC const MP_DEFINE_CONST_FUN_OBJ_2(math_add_obj, math_add);

STATIC const mp_rom_map_elem_t math_locals_dict_table[] = {
   {MP_ROM_QSTR(MP_QSTR_nothing), MP_ROM_PTR(&math_nothing_obj)},
   {MP_ROM_QSTR(MP_QSTR_add), MP_ROM_PTR(&math_add_obj)},
};

STATIC MP_DEFINE_CONST_DICT(math_locals_dict, math_locals_dict_table);

STATIC mp_obj_t modtest_math_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args)
{
    mp_arg_check_num(n_args, n_kw, 0,0, true);
    math_obj_t *self = m_new_obj(math_obj_t);
    self->base.type = &modtest_math_type;
    return MP_OBJ_FROM_PTR(self);
}

const mp_obj_type_t modtest_math_type = {
    .base = {&mp_type_type},
    .name = MP_QSTR_math,
    .make_new = modtest_math_make_new,
    .locals_dict = (mp_obj_dict_t *)&math_locals_dict,
};