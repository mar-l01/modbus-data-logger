from pymodbus.server.sync import StartTcpServer
from pymodbus.datastore import ModbusSequentialDataBlock, ModbusSlaveContext, ModbusServerContext


def run_server():
    mb_mapping = ModbusSlaveContext(
        di=ModbusSequentialDataBlock(1, [(True if i % 3 == 0 else False) for i in range(50)]),
        co=ModbusSequentialDataBlock(1, [0]*50),
        hr=ModbusSequentialDataBlock(1, [0]*50),
        ir=ModbusSequentialDataBlock(1, [i for i in range(50)]))

    context = ModbusServerContext(slaves=mb_mapping, single=True)

    StartTcpServer(context, address=("127.0.0.1", 5002))


if __name__ == "__main__":
    try:
        run_server()
    except KeyboardInterrupt:
        print("CTRL-C: Closing Modbus slave")
