from pymodbus.client.sync import ModbusTcpClient as ModbusClient

# default unit-id
UNIT_ID = 255

def run_modbus_master():
    client = ModbusClient("127.0.0.1", port=5002)
    client.connect()

    holding_reg_addr = 1
    holding_reg_val = 10
    print("Writing value {} to holding register at address {}...".format(holding_reg_val, holding_reg_addr))
    req = client.write_register(holding_reg_addr, holding_reg_val, unit=UNIT_ID)
    res = client.read_holding_registers(holding_reg_addr, 1, unit=UNIT_ID)
    if not req.isError() and res.registers[0] == holding_reg_val:
        print("...OK")
    else:
        print("...FAILED")

    holding_reg_vals = [10, 20, 30, 40, 50]
    print("Writing values {} to holding registers starting at address {}...".format(holding_reg_vals, holding_reg_addr))
    req = client.write_registers(holding_reg_addr, holding_reg_vals, unit=UNIT_ID)
    res = client.read_holding_registers(holding_reg_addr, len(holding_reg_vals), unit=UNIT_ID)
    if not req.isError() and res.registers == holding_reg_vals:
        print("...OK")
    else:
        print("...FAILED")

    client.close()


if __name__ == "__main__":
    run_modbus_master()
