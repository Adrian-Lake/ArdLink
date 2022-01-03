import serial as pys
from typing import List
from time import sleep
from .instruction import Instruction, load_instructions
from ArdLink import instruction

# Common Constants,  definition of these can be automated in the future
INSTR_BUFFER_OFFSET = 0x4000
DATA_BUFFER_OFFSET = 0x8000

instructions = load_instructions()


class Response:
    """Handle response from microcontroller for conversion
    into standard formats.
    """

    encoding = "UTF8"

    def __init__(self, response_bytes: bytes):

        self._bytes = response_bytes

    @property
    def bytes(self) -> bytes:
        """return response in bytes

        Returns:
            bytes: response data
        """

        return self._bytes

    @bytes.setter
    def bytes(self, value: bytes):
        self._bytes = value

    @property
    def str(self) -> str:
        """return response in str (UTF8)

        Returns:
            str: response data
        """
        return self._bytes.decode(self.encoding)


class Device(pys.Serial):
    """Interface class for micro controller firmware

    provides support for:
        connection - Extends serial.Serial
        instruction execution - self.execute
        instruction compilation - self.write_routine

    """

    minDelay = 0.05  # min delay between instruction execution

    def __init__(self, *args, **kwargs):

        super().__init__(*args, **kwargs)

    def execute(self, instruction: Instruction, get_response: bool = False) -> Response:
        """Execute instruction, collect all serial output within timeout period.

        Args:
            instruction (Instruction): Instruction to execute
            get_response (bool, optional): Get for response. Defaults to True.

        Returns:
            Response: response data if requested
        """

        self.write(instruction.bytes)

        if get_response:

            self.last_response = Response(self.readall())

            return self.last_response

        else:
            sleep(self.minDelay)

    def write_routine(
        self, routine: List[Instruction], get_response: bool = False
    ) -> Response:
        """Write a routine to the instruction buffer for later execution

        Args:
            routine ([Instruction]): set of instructions to execute
            get_response (bool, optional): Get for response. Defaults to False.


        Returns:
            Response: Response: response data if requested
        """

        routine_bytes = [  # caution, these are technically ints
            byte for instr in routine for byte in instr.bytes
        ]

        for n, b in enumerate(routine_bytes):
            self.execute(
                instructions["Set"](n + INSTR_BUFFER_OFFSET, b), get_response=False
            )  # Set word at specified address

        if get_response:

            self.last_response = Response(self.readall())

            return self.last_response

    def run_routine(self, *args, **kwargs):
        """Executes instruction buffer

        Returns:
            Response: Response: response data if requested
        """
        return self.execute(instructions["Set"](0x0001, 0x1), *args, **kwargs)
