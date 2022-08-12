# PYTHON 3.9.10

# IMPORTS
import tkinter as tk
import customtkinter as ctk

import parametros
import funciones
import widgets


# CLASES


class MainWindow:

    def __init__(self, ventana):
        self.wind = ventana
        # Título
        self.wind.title("Generador de Señales")
        # Dimensiones
        self.dimentions = (500, 600)
        self.wind.geometry(f"{self.dimentions[0]}x{self.dimentions[1]}")
        self.wind.maxsize(self.dimentions[0], self.dimentions[1])
        self.wind.minsize(self.dimentions[0], self.dimentions[1])
        # MenuBar
        self.menubar = widgets.barra_menu(ventana)
        # Channel 1
        self.ch1 = widgets.canal(ventana, 1)
        self.ch1.frame.grid(padx=(50, 50), pady=(25, 0))
        self.submit = widgets.submit(ventana, self.ch1)
        self.submit.boton.grid(pady=(100, 50))


if __name__ == '__main__':
    ctk.set_appearance_mode("system")  # Modes: system (default), light, dark
    ctk.set_default_color_theme("dark-blue")
    window = ctk.CTk()
    window.bind("<Button-1>", lambda event: event.widget.focus_set())
    app = MainWindow(window)
    window.mainloop()
