import customtkinter 
import math

# Configuración de la apariencia de la interfaz
customtkinter.set_appearance_mode("dark")  # Modos: "dark", "light", "system"
customtkinter.set_default_color_theme("blue")  # Temas : "blue", "green", "dark-blue"

class App(customtkinter.CTk) :
    def _init_(self) :
    super()._init_()

# --- CONFIGURACIÓN DE LA VENTANA PRINCIPAL ---
    self.title("Calculadora de Circuitos AC")
    self.geometry("450x350")
    self.resizable(False, False)

# --- CREAR EL SISTEMA DE PESTAÑAS ---
    self.tabview = customtkinter.CTkTabview(self, width = 420)
    self.tabview.pack(padx = 15, pady = 15, fill = "both", expand = True)

    self.tabview.add("Conversor Fasorial")
    self.tabview.add("Valor Eficaz (RMS)")

# --- LLAMAR A LOS MÉTODOS PARA POBLAR CADA PESTAÑA ---
    self.setup_phasor_tab()
    self.setup_rms_tab()

    def setup_phasor_tab(self) :
    """Crea todos los widgets para la pestaña de conversión a fasor."""
    tab = self.tabview.tab("Conversor Fasorial")

# --- WIDGETS DE ENTRADA ---
    phasor_vm_label = customtkinter.CTkLabel(tab, text = "Amplitud Máxima (Vm):")
    phasor_vm_label.grid(row = 0, column = 0, padx = 20, pady = (20, 10), sticky = "w")

    self.phasor_vm_entry = customtkinter.CTkEntry(tab, placeholder_text = "Ej: 120")
    self.phasor_vm_entry.grid(row = 0, column = 1, padx = 20, pady = (20, 10), sticky = "ew")

    phasor_phi_label = customtkinter.CTkLabel(tab, text = "Desfase (φ) en grados:")
    phasor_phi_label.grid(row = 1, column = 0, padx = 20, pady = 10, sticky = "w")

    self.phasor_phi_entry = customtkinter.CTkEntry(tab, placeholder_text = "Ej: -30")
    self.phasor_phi_entry.grid(row = 1, column = 1, padx = 20, pady = 10, sticky = "ew")

# --- BOTÓN DE CÁLCULO ---
    phasor_button = customtkinter.CTkButton(tab, text = "Calcular Fasor", command = self.calculate_phasor)
    phasor_button.grid(row = 2, column = 0, columnspan = 2, padx = 20, pady = 20)

# --- WIDGETS DE RESULTADO ---
    self.phasor_polar_result = customtkinter.CTkLabel(tab, text = "Forma Polar: -", font = ("Arial", 14))
    self.phasor_polar_result.grid(row = 3, column = 0, columnspan = 2, padx = 20, pady = (10, 5), sticky = "w")

    self.phasor_cartesian_result = customtkinter.CTkLabel(tab, text = "Forma Cartesiana: -", font = ("Arial", 14))
    self.phasor_cartesian_result.grid(row = 4, column = 0, columnspan = 2, padx = 20, pady = 5, sticky = "w")

    def setup_rms_tab(self) :
    """Crea todos los widgets para la pestaña de cálculo de valor eficaz."""
    tab = self.tabview.tab("Valor Eficaz (RMS)")

# --- WIDGETS DE ENTRADA ---
    rms_vm_label = customtkinter.CTkLabel(tab, text = "Amplitud Máxima (Vm):")
    rms_vm_label.grid(row = 0, column = 0, padx = 20, pady = (20, 10), sticky = "w")

    self.rms_vm_entry = customtkinter.CTkEntry(tab, placeholder_text = "Ej: 170")
    self.rms_vm_entry.grid(row = 0, column = 1, padx = 20, pady = (20, 10), sticky = "ew")

# --- BOTÓN DE CÁLCULO ---
    rms_button = customtkinter.CTkButton(tab, text = "Calcular Valor Eficaz", command = self.calculate_rms)
    rms_button.grid(row = 1, column = 0, columnspan = 2, padx = 20, pady = 20)

# --- WIDGET DE RESULTADO ---
    self.rms_result = customtkinter.CTkLabel(tab, text = "Valor Eficaz (RMS): -", font = ("Arial", 16))
    self.rms_result.grid(row = 2, column = 0, columnspan = 2, padx = 20, pady = 20)

    def calculate_phasor(self) :
    """Realiza el cálculo de conversión a fasor y actualiza la UI."""
    try :
    vm = float(self.phasor_vm_entry.get())
    phi_deg = float(self.phasor_phi_entry.get())

    # Convertir grados a radianes para los cálculos de cos y sin
    phi_rad = math.radians(phi_deg)

    # Calcular forma cartesiana
    real_part = vm * math.cos(phi_rad)
    imag_part = vm * math.sin(phi_rad)

    # Actualizar las etiquetas de resultado
    self.phasor_polar_result.configure(text = f"Forma Polar: {vm:.4f} ∠ {phi_deg}° V")
    self.phasor_cartesian_result.configure(text = f"Forma Cartesiana: {real_part:.4f} + j({imag_part:.4f}) V")

    except(ValueError, TypeError) :
    # Manejo de error si la entrada no es un número
    self.phasor_polar_result.configure(text = "Error: Ingrese valores numéricos.")
    self.phasor_cartesian_result.configure(text = "")

    def calculate_rms(self) :
    """Realiza el cálculo del valor eficaz y actualiza la UI."""
    try :
    vm = float(self.rms_vm_entry.get())

    # Para una señal sinusoidal, V_eficaz = Vm / sqrt(2)
    rms_value = vm / math.sqrt(2)

    # Actualizar la etiqueta de resultado
    self.rms_result.configure(text = f"Valor Eficaz (RMS): {rms_value:.4f} V")

    except(ValueError, TypeError) :
    # Manejo de error si la entrada no es un número
    self.rms_result.configure(text = "Error: Ingrese un valor numérico.")


    if _name_ == "_main_":
app = App()
app.mainloop()