from pymodbus.client.sync import ModbusTcpClient as ModbusClient

# default unit-id
UNIT_ID = 255

def run_modbus_master():
    client = ModbusClient("127.0.0.1", port=5002)
    client.connect()

    test_read_multiple_discrete_inputs(client)
    test_write_and_read_single_holding_register(client)
    test_write_and_read_multiple_holding_register(client)

    client.close()

def test_read_multiple_discrete_inputs(client):
    di_addr = 0
    di_nb = 8 # read bits will be padded by False until a byte is full
    # slave holds discrete input values [T, F, F, T, F, F, ..., F]
    di_expected = [(True if i % 3 == 0 else False) for i in range(50)][(di_addr) : (di_addr + di_nb)]

    print("Reading {} values from discrete inputs starting at address {}...".format(di_nb, di_addr))
    res = client.read_discrete_inputs(di_addr, di_nb, unit=UNIT_ID)
    if not res.isError() and res.bits == di_expected:
        print("...OK")
    else:
        print("...FAILED")

def test_write_and_read_single_holding_register(client):
    holding_reg_addr = 1
    holding_reg_val = 10

    print("Writing value {} to holding register at address {}...".format(holding_reg_val, holding_reg_addr))
    req = client.write_register(holding_reg_addr, holding_reg_val, unit=UNIT_ID)
    res = client.read_holding_registers(holding_reg_addr, 1, unit=UNIT_ID)

    if not req.isError() and res.registers[0] == holding_reg_val:
        print("...OK")
    else:
        print("...FAILED")

def test_write_and_read_multiple_holding_register(client):
    holding_reg_addr = 1
    holding_reg_vals = [10, 20, 30, 40, 50]

    print("Writing values {} to holding registers starting at address {}...".format(holding_reg_vals, holding_reg_addr))
    req = client.write_registers(holding_reg_addr, holding_reg_vals, unit=UNIT_ID)
    res = client.read_holding_registers(holding_reg_addr, len(holding_reg_vals), unit=UNIT_ID)

    if not req.isError() and res.registers == holding_reg_vals:
        print("...OK")
    else:
        print("...FAILED")

if __name__ == "__main__":
    run_modbus_master()
