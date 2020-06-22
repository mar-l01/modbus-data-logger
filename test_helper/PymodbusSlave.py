from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext


def run_server():
    mb_mapping = ModbusSlaveContext(
        di=ModbusSequentialDataBlock(0, [0]*10),
        co=ModbusSequentialDataBlock(0, [0]*10),
        hr=ModbusSequentialDataBlock(0, [0]*10),
        ir=ModbusSequentialDataBlock(0, [0]*10))

    context = ModbusServerContext(slaves=mb_mapping, single=True)

    StartTcpServer(context, address=("127.0.0.1", 5002))


if __name__ == "__main__":
    try:
        run_server()
    except KeyboardInterrupt:
        print("CTRL-C: Closing Modbus slave")
