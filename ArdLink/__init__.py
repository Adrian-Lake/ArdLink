from .device import Device

from .instruction import Instruction, load_instructions

# Load dynamic instruction classes into module
globals().update(load_instructions().items())


