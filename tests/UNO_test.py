import pytest
from time import sleep
import ArdLink as al


""" These tests involve communication with a real/simulated Arduino Uno and test the 
functionality of various features of the firmware.

For more information on Arduino simulation check out PicSimLab
https://lcgamboa.github.io/

"""


@pytest.fixture
def device(request):
    # Fixture to manage device connection

    # setup
    comms = al.Device(request.config.option.port, 9600, timeout=0.05)

    yield comms

    # teardown
    comms.close()


def test_temp_write(device):
    # Most basic function to test connection.
    # Write char to temp A then print it out.

    device.execute(al.Set(0x4012, 0x61))
    response = device.execute(al.PrintChar(0x4012), get_response=True)

    assert response.str == "a"


def test_data_buffer(device):
    # test write/read of data buffer

    data = "abc"

    for i, char in enumerate(data):
        device.execute(al.Set(al.device.DATA_BUFFER_OFFSET + i, ord(char)))

    for i, char in enumerate(data):
        device.execute(al.PrintChar(al.device.DATA_BUFFER_OFFSET + i))

    response = device.execute(al.NOP(), get_response=True)
    assert response.str == data


def test_instr_buffer(device):
    # test write/read/execution of data buffer

    device.write_routine(
        [
            al.Millis(),
            al.PrintChar(0x0010),
        ]
    )

    resp1 = device.run_routine(get_response=True).bytes

    sleep(0.01)

    resp2 = device.run_routine(get_response=True).bytes
    assert resp1 != resp2
