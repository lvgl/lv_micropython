#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "py/obj.h"
#include "py/runtime.h"
#include "py/mphal.h"
#include "pin.h"
#include "pin_static_af.h"
#include "mpu.h"
#include "systick.h"

#include "lib/oofatfs/ff.h"
#include "extmod/vfs.h"
#include "extmod/vfs_fat.h"

#include "py/objstr.h"
#include "py/objlist.h"
#include "py/stream.h"

#include "microbot_camera.h"
#include "microbot_led.h"


STATIC const mp_rom_map_elem_t mp_module_camera_globals_table[] = {
    { MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_camera) },
      { MP_ROM_QSTR(MP_QSTR_ov5640), MP_ROM_PTR(&camera_ov5640_type) },  
};

STATIC MP_DEFINE_CONST_DICT(mp_module_camera_globals, mp_module_camera_globals_table);

const mp_obj_module_t mp_module_camera = {
    .base = { &mp_type_module },
    .globals = (mp_obj_dict_t *)&mp_module_camera_globals
};

MP_REGISTER_MODULE(MP_QSTR_camera, mp_module_camera, 1);

