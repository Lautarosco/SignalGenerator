import tkinter as tk
import customtkinter as ctk
import funciones
import parametros

signals = ["Sine", "Triangular", "Square", "Sawtooth"]
global PUERTO


class cuadro_input:

    def __init__(self, frame, text, placehold, row, def_text=''):
        self.pad = 15
        self.texto = ctk.CTkLabel(frame, text=text, justify=tk.RIGHT)
        self.texto.grid(
            row=row,
            column=0,
            sticky='w',
            padx=self.pad,
            pady=self.pad)

        self.input = ctk.CTkEntry(frame,
                                  placeholder_text=f"{placehold}")
        self.input.insert(0, def_text)
        self.input.grid(
            row=row,
            column=1,
            padx=self.pad)


class submit:
    def __init__(self, frame, ch1):
        self.pad = 15

        self.boton = ctk.CTkButton(
            frame,
            text='SUBMIT',
            text_font="Helvetica 26",
            command=lambda: funciones.submit(ch1.get_values()),
            width=200,
            height=75)


class lista:
    def __init__(self, frame, sig, ch, row):
        self.list = ctk.CTkOptionMenu(frame,
                                      variable=ch.opc_lista,
                                      width=150,
                                      values=sig,
                                      )
        self.list.grid(row=row, column=0, pady=(20, 20), padx=(15, 15), columnspan=2)


class titulo:
    def __init__(self, frame, name, row):
        self.title = ctk.CTkLabel(frame, text=f"{name}", fg_color=("white", "RoyalBlue2"), text_font="Helvetica 40")
        self.title.grid(row=row, column=0, pady=(10, 20), padx=(5, 5), columnspan=2)


class canal:
    def __init__(self, wind, ch_number):
        self.ch_number = ch_number
        # Frame
        self.frame = ctk.CTkFrame(wind)
        # Título
        self.titulo = titulo(self.frame, f"Canal {ch_number}", row=0)
        # List
        self.opc_lista = tk.StringVar()
        self.lista = lista(self.frame, signals, self, row=1)
        # Frequency
        self.freq = cuadro_input(
            self.frame, 'Frequency (Hz)', f"{parametros.FREQ_MIN} - {parametros.FREQ_MAX}",
            def_text=parametros.FREQ_DEF, row=2)
        # Offset
        self.offset = ctk.CTkCheckBox(self.frame, text="Offset = 1V")
        self.offset.grid(row=3, columnspan=2, pady=(0, 20))

    def get_values(self):
        """ Devuelve un diccionario con los valores del canal """

        # signals = ["Sine", "Triangular", "Square", "Sawtooth"]

        table = dict([("Sine", 0),
                      ("Triangular", 1),
                      ("Square", 2),
                      ("Sawtooth", 3)
                      ])
        values = dict([('signal_id', table[self.lista.list.get()]),
                       ('freq', funciones.obtener_multiplo(self.freq.input.get())),
                       ('offset', self.offset.get()),
                       ])
        return values


class lista_puertos:
    def __init__(self, frame):
        self.cuadro = ctk.CTkFrame(frame)
        self.list = ctk.CTkOptionMenu(self.cuadro)
        self.list.grid(row=0, column=0)
        self.boton = ctk.CTkButton(self.cuadro, text="Refresh", command=funciones.listar_puertos)
        self.boton.grid(row=0, column=1)


class barra_menu:
    def __init__(self, frame):
        # Menubar
        self.menu = tk.Menu(frame)
        # TAB USB
        global PUERTO
        PUERTO = tk.StringVar()
        self.ports = tk.Menu(self.menu, tearoff=0)
        # Agrego el comando Actualizar
        self.ports.add_separator()
        self.ports.add_command(label='Actualizar', command=lambda: funciones.refresh_puertos(self.ports))
        # Busco los puertos disponibles
        funciones.refresh_puertos(self.ports)
        # Agrego Menubar
        self.menu.add_cascade(label="Puerto", menu=self.ports)

        frame.config(menu=self.menu)
