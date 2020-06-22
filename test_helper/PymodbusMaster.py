from pymodbus.client.sync import ModbusTcpClient as ModbusClient

def run_modbus_master():
    client = ModbusClient('localhost', port=5020)
    client.connect()

    holding_reg_addr = 1
    holding_reg_val = 10
    print("Writing value {} to holding register at address {}".format(holding_reg_addr, holding_reg_val))
    req = client.write_register(holding_reg_addr, holding_reg_val)
    res = client.read_holding_registers(holding_reg_addr, 1)
    assert(not req.isError())
    assert(res.registers[0] == holding_reg_val)

    holding_reg_vals = [10,20,30,40,50]
    print("Writing values {} to holding registers starting at address {}".format(holding_reg_addr, holding_reg_vals))
    req = client.write_registers(holding_reg_addr, holding_reg_vals)
    res = client.read_holding_registers(holding_reg_addr, len(holding_reg_vals))
    assert(not req.isError())
    assert(res.registers == holding_reg_vals)

    client.close()


if __name__ == "__main__":
    run_modbus_master()
