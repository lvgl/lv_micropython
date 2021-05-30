#include "stdint.h"
#include "stdio.h"

#include "py/obj.h"
#include "py/runtime.h"

#include "modtest_math.h"

STATIC mp_obj_t modtest_test0()
{
    printf("this is the modtest function: test0 \n");
    return mp_const_none;
}

STATIC mp_obj_t modtest_test1(mp_obj_t data)
{
    printf("this function has one parameter: %d \n", mp_obj_get_int(data));
    return mp_const_none;
}

STATIC const MP_DEFINE_CONST_FUN_OBJ_0(modtest_obj_test0, modtest_test0);
STATIC const MP_DEFINE_CONST_FUN_OBJ_1(modtest_obj_test1, modtest_test1);


STATIC const mp_rom_map_elem_t modtest_globals_table[] = {
    {MP_OBJ_NEW_QSTR(MP_QSTR__name__), MP_ROM_QSTR(MP_QSTR_modtest)},
    {MP_OBJ_NEW_QSTR(MP_QSTR_test0), MP_ROM_PTR(&modtest_obj_test0)},
    {MP_OBJ_NEW_QSTR(MP_QSTR_test1), MP_ROM_PTR(&modtest_obj_test1)},
    {MP_OBJ_NEW_QSTR(MP_QSTR_math), MP_ROM_PTR(&modtest_math_type)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_modtest_globals, modtest_globals_table);

const mp_obj_module_t mp_module_modtest = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&mp_module_modtest_globals,
};