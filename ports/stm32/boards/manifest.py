include("$(MPY_DIR)/extmod/asyncio")

require("dht")
require("onewire")
freeze("$(MPY_DIR)/lib/micropython-lib/micropython/drivers/display/lcd160cr", ("lcd160cr.py", "lcd160cr_test.py"))
freeze("$(MPY_DIR)/lib/lv_bindings/lib", "lv_utils.py")
