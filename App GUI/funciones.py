import tkinter

import parametros
import widgets
import serial.tools.list_ports

global PUERTO


# Botones


def obtener_multiplo(numero):
    """ Convierte un texto formado por: <int><str> al número correspondiente """
    powers = {'k': 10 ** 3, 'm': 10 ** 6}
    last_dig = numero[-1].lower()
    num_strip = numero[:-1]
    try:
        if last_dig in powers.keys():
            return int(float(num_strip) * powers[last_dig])
        else:
            return int(float(numero))
    except Exception as E:
        print(f"INVALID_INPUT: {numero}")
        print(E)
        return


def extraer_texto(entry):
    """  """
    num = obtener_multiplo(entry.get())
    if isinstance(num, int):
        if num > parametros.FREQ_MAX:
            print(f"MAX_FREQ_EXCEEDED (limit : {parametros.FREQ_MAX} , got : {num})")
            return parametros.FREQ_MAX
        elif num < parametros.FREQ_MIN:
            return parametros.FREQ_MIN
        else:
            return num


# UART


def verify(channel: widgets.canal):
    """ Verifica si los datos de un canal son válidos."""
    print(f'Veryficing channel {channel.ch_number}...')
    return channel


def convert(ch1: dict):
    """ Convierte los datos de dos canales a una string para enviar al micro."""
    m1 = f"{ch1['signal_id']}-{ch1['freq']}-{ch1['offset']}"
    return m1


def send(message):
    if PUERTO.get() != 'None':
        ser = serial.Serial(PUERTO.get(), parametros.BAUDRATE)
        data = ('[' + message + ']').ljust(31) + '\0'
        data = data.encode(encoding='utf-8')
        print(f'Sending {data} to port {PUERTO.get()}...')
        ser.write(data)
    else:
        print("No port selected.")


def submit(ch1):
    # Convierte información en string para mandar por UART
    msg = convert(ch1)
    # Enviarlo por puerto serie
    send(msg)


def listar_puertos():
    ports = serial.tools.list_ports.comports(include_links=False)
    return ports


def refresh_puertos(menu):
    print('refreshing...')
    # Deselecciono el puerto actual
    global PUERTO
    PUERTO = tkinter.StringVar()
    PUERTO.set('None')
    # Me fijo cuántos puertos hay
    items = menu.index('Actualizar') - 1
    # Si hay uno o más, los borro
    if items > 0:
        menu.delete(0, items - 1)
        print('borrando')
    for port in listar_puertos():
        menu.insert_radiobutton(index=0, label=port.device, value=port.device, variable=PUERTO)


def serupdate(root):
    global PUERTO
    if PUERTO.get() != "None":
        ser = serial.Serial(PUERTO.get(), parametros.BAUDRATE)
        texto = "".join(iter(lambda: ser.read(1), "\n"))
        print(f"DEVICE: {texto}")
    root.after(1000, lambda: serupdate(root))
