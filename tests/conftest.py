

def pytest_addoption(parser):
    parser.addoption('--port', action="store", default="COM2", help="Target serial port")

