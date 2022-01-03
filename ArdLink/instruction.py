from enum import Enum
import re
from pathlib import Path

## Call formats for different instruction
def instr_type_call(self):
    pass


def instr_type_one_param(self, parameter):
    self.p1 = parameter
    self.p2 = 0


def instr_type_two_param(self, parameter_1, parameter_2):
    self.p1 = parameter_1
    self.p2 = parameter_2


# Microcontroller types
class Target(Enum):
    OTHER = {}
    ARDUINO = {"endian": "little"}


class Instruction:
    """Class for converting individual instructions to bytecode
    for execution or storage.

    """

    # default class attributes
    op = 0
    p1 = 0
    p2 = 0

    def __init__(self, opcode: int, parameter_1: int, parameter_2: int):

        self.op = opcode
        self.p1 = parameter_1
        self.p2 = parameter_2

    @staticmethod
    def encode_bytes(data: int, byteorder: str = "little") -> bytes:
        """Encoder to convert int data to bytes in correct endian."""

        return data.to_bytes(2, byteorder)

    @property
    def bytes(self, target: Target = Target.ARDUINO) -> bytes:
        """Get bytecode for instruction.

        Args:
            target (Target, optional): Target to compile for, sets parameters such as endian. Defaults to Target.ARDUINO.

        Returns:
            bytes: instruction bytecode
        """

        instr_set = [self.op, self.p1, self.p2]
        # get endian spec
        endian = target.value.get("endian")

        # Fun list comprehension
        instr_bytes = bytes(
            [
                byte
                for instr in instr_set
                for byte in self.encode_bytes(instr, byteorder=endian)
            ]
        )

        return instr_bytes


def instruction_factory(
    name: str = None,
    opcode: int = None,
    doc_string: str = "",
    init_func=instr_type_two_param,
):
    """Factory for creating instruction classes for each opcode.

    Args:
        name (str, optional): Class name.
        opcode (int, optional): operation code.
        doc_string (str, optional): [description]. Defaults to "".
        init_func ([type], optional): [description]. Defaults to instr_type_two_param.

    Returns:
        [Instruction subclass]: Class for compiling instruction bytecode
    """

    attributes = {"op": opcode, "__init__": init_func, "__doc__": doc_string}

    return {name: type(name, (Instruction,), attributes)}


def load_instructions(source: str = None) -> dict:
    """Generate Instruction class set from source

    In the future the output of this function should be cached.

    Args:
        source (str, optional): Source to parse opcodes from. Defaults to firmware\interpreter.h


    Returns:
        dict: Instructions classes
    """
    if source is None:
        # read file
        with open(
            Path(__file__).parent.joinpath("./firmware/interpreter.h"), "r"
        ) as fh:
            source = fh.read()

    # flags for parsing header
    begin_op_definition = False
    end_op_definition = False
    op_code_bytes = None

    # instruction definitions from header
    instruction_defs = []

    # parse header file
    for n, line in enumerate(source.splitlines()):

        # look for section declaration
        section = re.search(r"//section:\s*(?P<Section>\S+)\s*", line, re.IGNORECASE)

        # if reading section parse!
        if section:
            begin_op_definition |= (
                "begin_op_definition" == section.groupdict()["Section"].lower()
            )
            end_op_definition |= (
                "end_op_definition" == section.groupdict()["Section"].lower()
            )

        if end_op_definition:
            break

        if begin_op_definition:

            op_code = re.search(
                r"case\s+0x(?P<opcode>[a-fA-F0-9]+)", line, re.IGNORECASE
            )
            op_doc = re.search(
                r"sym:\s*(?P<sym>\S*)\s+params:\s*(?P<params>\d*)\s+doc_str:\s*(?P<doc_str>.*)\s*$",
                line,
                re.IGNORECASE,
            )

            if not op_code is None:
                op_code_bytes = int(op_code.groupdict()["opcode"], 16)

            if (not op_doc is None) and (not op_code_bytes is None):

                op_d = op_doc.groupdict()

                instruction_defs.append(
                    {
                        "name": op_d["sym"],
                        "opcode": op_code_bytes,
                        "doc_string": op_d["doc_str"],
                        "init_func": [
                            instr_type_call,
                            instr_type_one_param,
                            instr_type_two_param,
                        ][int(op_d["params"])],
                    }
                )

    instructions = {}

    # Send definitions to factory for class generation
    for instr_def in instruction_defs:
        instr = instruction_factory(**instr_def)
        instructions.update(instr)

    return instructions
