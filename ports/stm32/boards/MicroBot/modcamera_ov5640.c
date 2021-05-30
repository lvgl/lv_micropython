#include "stdint.h"
#include "stdio.h"
#include "py/obj.h"
#include "py/runtime.h"

#include "microbot_camera.h"
#include "microbot_led.h"

typedef struct _camera_ov5640_obj_t {
    mp_obj_base_t base;
		uint16_t camera_id;
}ov5640_obj_t;

#define __ATTR_VIDEOBUFFER  __attribute__ ((section(".videosdram"))) __attribute__ ((aligned (4)))
uint32_t ov5640_fb[320 * 240 /2]  __ATTR_VIDEOBUFFER;

STATIC mp_obj_t camera_ov5640_deinit() 
{

    BSP_CAMERA_DeInit();
    
    BSP_LED_BLUE(0);
    BSP_LED_RED(0);
    BSP_LED_GREEN(0);
    
    BSP_LED_GPIO_DeInit();

    return mp_const_none;
}

STATIC mp_obj_t camera_ov5640_init() 
{

    BSP_LED_GPIO_Init();
    BSP_LED_BLUE(0);
    BSP_LED_RED(0);
    BSP_LED_GREEN(0);
    
    uint16_t ret_val = 0;

    ret_val = BSP_CAMERA_Init();
    printf("Camera INIT: %d \n ", ret_val);
 
    return mp_const_none;
}


//def camera_start()
STATIC mp_obj_t camera_ov5640_start(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {

    mp_int_t ret_val = 0;

    /* Your code start! */
    ret_val = BSP_CAMERA_Start(DCMI_MODE_CONTINUOUS, (uint32_t)ov5640_fb);
    /* Your code end! */
    //lv_ex_canvas_1();
    return mp_obj_new_int(ret_val); 
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(camera_ov5640_deinit_obj, camera_ov5640_deinit);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(camera_ov5640_init_obj, camera_ov5640_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(camera_ov5640_start_obj, 0, camera_ov5640_start);

STATIC void camera_ov5640_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) 
{
	ov5640_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "<ov5640 ID:%x>", self -> camera_id);
}
//----------------------------------------------------------------------------------
STATIC mp_obj_t camera_ov5640_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) 
{

	// check arguments
	mp_arg_check_num(n_args, n_kw, 0, MP_OBJ_FUN_ARGS_MAX, true);

    ov5640_obj_t *self = m_new_obj(ov5640_obj_t);
    self -> base.type = &camera_ov5640_type;
    self -> camera_id = 0;
    return MP_OBJ_FROM_PTR(self);
}

/******************************************************************************/
STATIC const mp_rom_map_elem_t camera_ov5640_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR__name__), MP_ROM_QSTR(MP_QSTR_ov5640) },
    { MP_ROM_QSTR(MP_QSTR_deinit), MP_ROM_PTR(&camera_ov5640_deinit_obj) },
    { MP_ROM_QSTR(MP_QSTR_init), MP_ROM_PTR(&camera_ov5640_init_obj) },
    { MP_ROM_QSTR(MP_QSTR_start), MP_ROM_PTR(&camera_ov5640_start_obj) },
};

STATIC MP_DEFINE_CONST_DICT(camera_ov5640_locals_dict, camera_ov5640_locals_dict_table);

const mp_obj_type_t camera_ov5640_type = {
    .base = { &mp_type_type },
    .name = MP_QSTR_ov5640,
    .print = camera_ov5640_print,
    .make_new = camera_ov5640_make_new,
    .locals_dict = (mp_obj_dict_t *)&camera_ov5640_locals_dict,
};
