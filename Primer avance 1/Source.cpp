//Primer avance Estructura de datos Jose de Jesus De la Cruz Santiago

#include <Windows.h>
#include "resource.h"
#include <CommDlg.h>
#include <fstream>
#include <Commctrl.h>
#include <stdio.h>

using namespace std;

//booleano para validaciones
bool bol=true;
bool imagenCargada = false;

//Entidades

struct usuario {
	usuario* ant;
	char clave[30];
	char ApellidoP[50];
	char ApellidoM[50];
	char nomUsu[50];
	char contra[30];
	usuario* sig;
};
usuario* aux1, * inicio1 = nullptr;

struct vacunas {
	vacunas* ant;
	char tipo[50];
	char marca[50];
	char NDosis[10];
	char ClaveVac[50];
	char Descrip[60];
	char precio[30];
	vacunas* sig;

};
vacunas* aux2, * inicio2 = nullptr;

struct personas {
	personas* ant;
	char ApellidoPP[50];
	char ApellidoMP[50];
	char Nombres[50];
	char CURP[50];
	char RFC[50];
	SYSTEMTIME FechaNac;
	char Calle[50];
	char Colonia[50];
	char Ciudad[50];
	char Estado[50];
	int EstadoCiv;
	char telefono[20];
	char Doc[MAX_PATH];
	int Sexo;
	char GrupoOcup[30];
	int PerfilRies;
	personas* sig;

};
personas* aux3, * inicio3 = nullptr;

struct carnet {
	carnet* ant;
	SYSTEMTIME FechaVac;
	char Lote[20];
	char CentroVac[50];
	char IDVac[50];
	char CURPPerson[50];
	char ClaveVacCarnet[50];
	carnet* sig;
};
carnet* aux4, * inicio4, * fin4= nullptr;

char zFile[MAX_PATH];


//Ventanas principales
LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK VentanaRegistrarUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//dar de alta
LRESULT CALLBACK AltaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaPacientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK AltaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//lista
LRESULT CALLBACK ListaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaPacientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ListaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Registrar
void RegistrarUsuario(usuario* nuevo);
void RegistrarVacunas(vacunas* nuevo);
void RegistrarPersonas(personas* nuevo);
void RegistrarCarnet(carnet* nuevo);

//Verificacion
bool IDExists(const char* id);

//modificar
void ModificarVacunas(vacunas* nuevaInfo_Vac, char* textoList);
void ModificarPacientes(personas* nuevaInfo_Pac, char* textoListPac);
void ModificarCarnet(carnet* nuevaInfo_Car, char* textoListCar);

//Mostrar Info
LRESULT CALLBACK MostrarInfoPac(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MostrarInfoVac(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

//Elimar 
void EliminarVac(char nomVac[50]);
void EliminarPac(char nomPac[50]);
void EliminarCar(char nomCar[50]);

//Escribir info en txt
void EscribirUsuario();
void EscribirVacunas();
void EscribirPersonas();

//Escribir info en archivo bin
void EscribirUsuBin();
void EscribirVacBin();
void EscribirPacBin();
void EscribirCarBin();

//Leer la indo del archivo bin
void LeerUsubina();
void LeerVacbina();
void LeerPacbina();
void LeerCarbina();

//busqueda binaria
carnet* buscarPorID(carnet* inicio, carnet* fin, const char* idBuscado);

//Ordednamiento
void AgregarElementosListBox(HWND hwndDlg, carnet* inicio, bool ordenar /*= false*/);
void heapify(carnet** arr, int n, int i);

//Ordenar por quicksort
void AgregarElementosListBoxCURP(HWND hwndDlg, carnet* inicio, bool ordenar);
void swap(carnet** arr, int i, int j);
int partition(carnet** arr, int low, int high);
void quicksort(carnet** arr, int low, int high);
void CrearArchivoCSVQuick(carnet** arr, int numElementos);
void ReporteQuickCSV(HWND hwndDlg, carnet* inicio, bool ordenar);


HINSTANCE hInstGlobal;
HWND hPrinGlob;

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {

	LeerUsubina();
	LeerVacbina();
	LeerPacbina();
	LeerCarbina();

	hInstGlobal = hInst;
	HWND hVInicio = CreateDialog(hInst, MAKEINTRESOURCE(IDD_IniciarSesion), NULL, VentanaInicio);

	ShowWindow(hVInicio, cmdshow);

	MSG msg;
	ZeroMemory(&msg, sizeof(msg));

	while (GetMessage(&msg, NULL, NULL, NULL)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	EscribirUsuBin();
	EscribirVacBin();
	EscribirPacBin();
	EscribirCarBin();

	return msg.wParam;
}

//Ventanas principales 


LRESULT CALLBACK VentanaInicio(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam))  {
		case ID_IniciarSe: {

			char clave[30];
			char contra[30];
			GetDlgItemText(hwnd, IDC_Clave, clave, sizeof(clave));
			GetDlgItemText(hwnd, IDC_Contra, contra, sizeof(contra));


			aux1 = inicio1;
			if (inicio1 == nullptr) {
				MessageBox(hwnd, "No hay usuarios registrados.", "AVISO", MB_OK | MB_ICONERROR);
			}
			else {
				while (aux1 != nullptr && strcmp(aux1->clave, clave) != 0) {
					aux1 = aux1->sig;
				}
				if (aux1 == nullptr) {
					MessageBox(hwnd, "Usuario no encontrado", "AVISO", MB_OK | MB_ICONERROR);
				}
				else {
					if (strcmp(aux1->contra, contra) == 0) {
						inicio1 = aux1;
						EndDialog(hwnd, 0);
						DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_VentanaPrincipal), NULL, VentanaPrincipal);
					}
					else {
						MessageBox(hwnd, "Contraseña incorrecta", "AVISO", MB_OK | MB_ICONERROR);
					}
				}
			}

		}
				break;

		case IDCANCEL: {
			int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (opc == IDYES) {
				DestroyWindow(hwnd);
			}
		}
				break;

		case IDC_CrearUsu: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(105), hwnd, VentanaRegistrarUsu);
		}
				break;
		}
	}
	break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (opc == IDYES) {
			DestroyWindow(hwnd);
		}
	}
				break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}
				break;

	}
	return false;
}
LRESULT CALLBACK VentanaPrincipal(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_CerrarSesion: {
			int opc = MessageBox(hwnd, "¿Seguro que desea cerrar sesion?", "AVISO", MB_YESNO | MB_ICONQUESTION);
			if (opc == IDYES) {
				EndDialog(hwnd, 0);
				DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_IniciarSesion), NULL, VentanaInicio);
			}
		}
		break;

		case ID_VACUNAS_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_AltaVacunas), NULL, AltaVacunas);
		}
		break;

		case ID_PACIENTES_ALTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_AltaPaciente), NULL, AltaPacientes);
		}
		break;

		case ID_CARNET_ALTA: {

			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_AltaCarnet), NULL, AltaCarnet);

		}
		break;

		case ID_VACUNAS_LISTA: {

			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_ListaVacunas), NULL, ListaVacunas);
		}
		break;

		case ID_PACIENTES_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_ListaPacientes), NULL, ListaPacientes);
		}
		break;

		case ID_CARNET_LISTA: {
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_ListaCarnet), NULL, ListaCarnet);
		}
		break;

		}
	}
	break;

	case WM_INITDIALOG: {
			SetDlgItemText(hwnd, IDC_NombUsu, aux1->nomUsu);
			SetDlgItemText(hwnd, IDC_ApellidoP, aux1->ApellidoP);
			SetDlgItemText(hwnd, IDC_ApellidoM, aux1->ApellidoM);
			SetDlgItemText(hwnd, IDC_ClaveUsu, aux1->clave);
	}
	break;

	case WM_CLOSE: {
		int opc = MessageBox(hwnd, "¿Seguro que desea salir?", "AVISO", MB_YESNO | MB_ICONQUESTION);
		if (opc == IDYES) {
			DestroyWindow(hwnd);
		}
	}
				 break;

	case WM_DESTROY: {
		PostQuitMessage(0);
	}
				   break;

	}
	return false;
	}
LRESULT CALLBACK VentanaRegistrarUsu(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case 1015: {
			bool bol = true;
			usuario* newUser = new usuario;
			GetDlgItemText(hwnd, IDC_NewNomb, newUser->nomUsu, sizeof(newUser->nomUsu));
			GetDlgItemText(hwnd, IDC_ApellidoP, newUser->ApellidoP, sizeof(newUser->ApellidoP));
			GetDlgItemText(hwnd, IDC_ApellidoM, newUser->ApellidoM, sizeof(newUser->ApellidoM));
			GetDlgItemText(hwnd, IDC_ClaveUsu, newUser->clave, sizeof(newUser->clave));
			GetDlgItemText(hwnd, IDC_ContraUsu, newUser->contra, sizeof(newUser->contra));

			for (int i = 0; i < strlen(newUser->nomUsu); i++) {
				if (!isalpha(newUser->nomUsu[i]) && newUser->nomUsu[i] != ' ') {
					MessageBox(hwnd, "Los nombres/apellidos del usuario son inválidos: solo se permiten carácteres alfabéticos y espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}
			for (int i = 0; i < strlen(newUser->ApellidoP); i++) {
				if (!isalpha(newUser->ApellidoP[i]) && newUser->ApellidoP[i] != ' ') {
					MessageBox(hwnd, "Los nombres/apellidos del usuario son inválidos: solo se permiten carácteres alfabéticos y espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}
			for (int i = 0; i < strlen(newUser->ApellidoM); i++) {
				if (!isalpha(newUser->ApellidoM[i]) && newUser->ApellidoM[i] != ' ') {
					MessageBox(hwnd, "Los nombres/apellidos del usuario son inválidos: solo se permiten carácteres alfabéticos y espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			if (strlen(newUser->nomUsu) < 4) {
				MessageBox(hwnd, "El nombre de usuario es inválido: el mínimo de carácteres es 4", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
				break;
			}
			if (strlen(newUser->clave) < 5) {
				MessageBox(hwnd, "La clave de usuario es inválido: el mínimo de carácteres es 5", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
				break;
			}
			if (strlen(newUser->ApellidoP) < 2) {
				MessageBox(hwnd, "Falto colocar apellidos", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
				break;
			}
			if (strlen(newUser->ApellidoM) < 2) {
				MessageBox(hwnd, "Falto colocar apellidos", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
				break;
			}

			bool requisitos1 = false;
			bool requisitos2 = false;
			bool requisitos3 = false;
			for (int i = 0; newUser->contra[i] != '\0'; i++) {
				if (isalpha(newUser->contra[i])) {
					requisitos1 = true;
				}
				if (isdigit(newUser->contra[i])) {
					requisitos2 = true;
				}
				if (ispunct(newUser->contra[i])) {
					requisitos3 = true;
				}
			}
			if (!requisitos1 || !requisitos2 || !requisitos3) {
				MessageBox(hwnd, "La contraseña es inválida: debe incluir al menos una letra, un número, y un carácter especial", "AVISO", MB_OK | MB_ICONERROR);
				break;
			}

			aux1=inicio1;
			bool claveRepetida = false;
			while (aux1 != nullptr) {
				if (strcmp(aux1->clave, newUser->clave) == 0) {
					claveRepetida = true;
					MessageBox(hwnd, "La clave de usuario ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
				aux1 = aux1->sig;
			}

			for (int i = 0; i < strlen(newUser->clave); i++) {
				if (newUser->clave[i] == ' ') {
					MessageBox(hwnd, "La clavede usuario no puede contener espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			if (bol==true) {
				RegistrarUsuario(newUser);
				delete newUser;
				SetDlgItemText(hwnd, IDC_NewNomb, "");
				SetDlgItemText(hwnd, IDC_ApellidoP, "");
				SetDlgItemText(hwnd, IDC_ApellidoM, "");
				SetDlgItemText(hwnd, IDC_ClaveUsu, "");
				SetDlgItemText(hwnd, IDC_ContraUsu, "");
			}

		}
				break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
			DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_IniciarSesion), NULL, VentanaInicio);
		}
				break;
		}
	}
			break;
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
		DialogBox(hInstGlobal, MAKEINTRESOURCE(IDD_IniciarSesion), NULL, VentanaInicio);
	}
			break;

	}
	return false;
	}

//Dar de alta

LRESULT CALLBACK AltaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK: {
			bool bol = true;
			float precio;
			vacunas* newVac = new vacunas;
			GetDlgItemText(hwnd, IDC_TipoVac, newVac->tipo, sizeof(newVac->tipo));
			GetDlgItemText(hwnd, IDC_MarcaVac, newVac->marca, sizeof(newVac->marca));
			GetDlgItemText(hwnd, IDC_NoDosis, newVac->NDosis, sizeof(newVac->NDosis));
			GetDlgItemText(hwnd, IDC_ClaseVac, newVac->ClaveVac, sizeof(newVac->ClaveVac));
			GetDlgItemText(hwnd, IDC_Desc, newVac->Descrip, sizeof(newVac->Descrip));
			GetDlgItemText(hwnd, IDC_Precio, newVac->precio, sizeof(newVac->precio));

			for (int i = 0; i < strlen(newVac->precio); i++) {
				if (!isdigit(newVac->precio[i]) && newVac->precio[i] != ' ') {
					MessageBox(hwnd, "Precio invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}
			for (int i = 0; i < strlen(newVac->NDosis); i++) {
				if (!isdigit(newVac->NDosis[i]) && newVac->NDosis[i] != ' ') {
					MessageBox(hwnd, "El numero de dosis debe ser en formato numerico", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			aux2 = inicio2;
			bool ClaveRepetida = false;
			while (aux2 != nullptr) {
				if (strcmp(aux2->ClaveVac, newVac->ClaveVac) == 0) {
					ClaveRepetida = true;
					MessageBox(hwnd, "La clave de usuario ya está en uso.", "AVISO", MB_OK | MB_ICONERROR);
					break;
				}
				aux2 = aux2->sig;
			}

			if (bol) {
				precio = GetDlgItemInt(hwnd, IDC_Precio, NULL, false);

				if(precio > 0){
					RegistrarVacunas(newVac);
					delete newVac;
					SetDlgItemText(hwnd, IDC_TipoVac, "");
					SetDlgItemText(hwnd, IDC_MarcaVac, "");
					SetDlgItemText(hwnd, IDC_NoDosis, "");
					SetDlgItemText(hwnd, IDC_ClaseVac, "");
					SetDlgItemText(hwnd, IDC_Desc, "");
					SetDlgItemText(hwnd, IDC_Precio, "");
				}
				else
				{
					MessageBox(hwnd, "Solo se aceptan precios mayores a 0", "AVISO", MB_OK | MB_ICONERROR);
				}
			}

		}
		break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		}
		break;

		}
	}break;

	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
	break;
	
	}
	return false;
}
LRESULT CALLBACK AltaPacientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		char EstadoCivil[7][35] = { "1. Casado(a)", "2. Conviviente", "3. Anulado(a)", "4. Separado de unión legal","5. Separado(a) de unión de hecho", "6. Viudo(a)", "7. Soltero(a)"};
		for (int i = 0; i < 7; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOEstadoC, CB_INSERTSTRING, (WPARAM)i, (LPARAM)EstadoCivil[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)0, 0);

		char PerfilRiesg[3][10] = { "Alto", "Medio", "Bajo" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOPerfilDR, CB_INSERTSTRING, (WPARAM)i, (LPARAM)PerfilRiesg[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)0, 0);

	}
	break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK:  {
				bool bol = true;
				personas* newPac = new personas;
				GetDlgItemText(hwnd, IDC_ApellidoPP, newPac->ApellidoPP, sizeof(newPac->ApellidoPP));
				GetDlgItemText(hwnd, IDC_ApellidoMP, newPac->ApellidoMP, sizeof(newPac->ApellidoMP));
				GetDlgItemText(hwnd, IDC_NombresP, newPac->Nombres, sizeof(newPac->Nombres));
				GetDlgItemText(hwnd, IDC_CURP, newPac->CURP, sizeof(newPac->CURP));
				GetDlgItemText(hwnd, IDC_RFC, newPac->RFC, sizeof(newPac->RFC));
				GetDlgItemText(hwnd, IDC_Calle, newPac->Calle, sizeof(newPac->Calle));
				GetDlgItemText(hwnd, IDC_Colonia, newPac->Colonia, sizeof(newPac->Colonia));
				GetDlgItemText(hwnd, IDC_Ciudad1, newPac->Ciudad, sizeof(newPac->Ciudad));
				GetDlgItemText(hwnd, IDC_Estado1, newPac->Estado, sizeof(newPac->Estado));
				GetDlgItemText(hwnd, IDC_Telefono, newPac->telefono, sizeof(newPac->telefono));
				GetDlgItemText(hwnd, IDC_GrupoO, newPac->GrupoOcup, sizeof(newPac->GrupoOcup));

				SYSTEMTIME fechaSeleccionada;
				ZeroMemory(&fechaSeleccionada, sizeof(fechaSeleccionada));
				DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_FechaNaci), &fechaSeleccionada);

				SYSTEMTIME fechaHoy;
				ZeroMemory(&fechaHoy, sizeof(fechaHoy));
				GetLocalTime(&fechaHoy);

				if (fechaSeleccionada.wYear > fechaHoy.wYear ||
					(fechaSeleccionada.wYear == fechaHoy.wYear && fechaSeleccionada.wMonth > fechaHoy.wMonth) ||
					(fechaSeleccionada.wYear == fechaHoy.wYear && fechaSeleccionada.wMonth == fechaHoy.wMonth && fechaSeleccionada.wDay > fechaHoy.wDay)) {
					MessageBox(hwnd, "Fecha invalida: no puedes seleccionar una fecha en el futuro", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}

				newPac->FechaNac = fechaSeleccionada;

				if (IsDlgButtonChecked(hwnd, IDC_RHombre) == BST_CHECKED) {
					newPac->Sexo = 1;
				}
				if (IsDlgButtonChecked(hwnd, IDC_RMujer) == BST_CHECKED) {
					newPac->Sexo = 2;
				}

				if (strlen(newPac->telefono) != 10) {
					MessageBox(hwnd, "Telefono invalido: requiere 10 digitos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}
				for (int i = 0; i < strlen(newPac->telefono); i++) {
					if (!isdigit(newPac->telefono[i]) && newPac->telefono[i] != ' ') {
						MessageBox(hwnd, "Telefono invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
						bol = false;
					}
				}

				int EstadoCivilSelect = 0;
				EstadoCivilSelect = SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_GETCURSEL, 0, 0);
				newPac->EstadoCiv = EstadoCivilSelect + 1;

				int PerfilRSelect = 0;
				PerfilRSelect = SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_GETCURSEL, 0, 0);
				newPac->PerfilRies = PerfilRSelect + 1;

				//validar curp y rfc
				if (strlen(newPac->CURP) != 18) {
					MessageBox(hwnd, "CURP invalido: requiere 18 elementos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}
				if (strlen(newPac->RFC) != 13) {
					MessageBox(hwnd, "RFC invalido: requiere 13 elementos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}

				aux3 = inicio3;
				bool CURPrepetido = false;
				while (aux3 != nullptr) {
					if (strcmp(aux3->CURP, newPac->CURP) == 0) {
						CURPrepetido = true;
						MessageBox(hwnd, "La CURP del paciente ya está registrada.", "AVISO", MB_OK | MB_ICONERROR);
						bol = false;
						break;
					}
					aux3 = aux3->sig;
				}

				aux3 = inicio3;
				bool RFCrepetido = false;
				while (aux3 != nullptr) {
					if (strcmp(aux3->RFC, newPac->RFC) == 0) {
						RFCrepetido = true;
						MessageBox(hwnd, "El RFC del paciente ya está registrado.", "AVISO", MB_OK | MB_ICONERROR);
						bol = false;
						break;
					}
					aux3 = aux3->sig;
				}

				if (!imagenCargada) {
					MessageBox(hwnd, "Cargue un documento valido", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}
				else {
					bol = true;
				}

				for (int i = 0; i < strlen(newPac->CURP); i++) {
					if (newPac->CURP[i] == ' ') {
						MessageBox(hwnd, "El CURP no puede contener espacios", "AVISO", MB_OK | MB_ICONERROR);
						bol = false;
						break;
					}
				}
				for (int i = 0; i < strlen(newPac->RFC); i++) {
					if (newPac->RFC[i] == ' ') {
						MessageBox(hwnd, "El RFC no puede contener espacios", "AVISO", MB_OK | MB_ICONERROR);
						bol = false;
						break;
					}
				}

				strcpy_s(newPac->Doc, zFile);

				if (bol == true) {
					RegistrarPersonas(newPac);
					delete newPac;
					SetDlgItemText(hwnd, IDC_ApellidoPP, "");
					SetDlgItemText(hwnd, IDC_ApellidoMP, "");
					SetDlgItemText(hwnd, IDC_NombresP, "");
					SetDlgItemText(hwnd, IDC_CURP, "");
					SetDlgItemText(hwnd, IDC_RFC, "");
					SetDlgItemText(hwnd, IDC_Calle, "");
					SetDlgItemText(hwnd, IDC_Colonia, "");
					SetDlgItemText(hwnd, IDC_Ciudad1, "");
					SetDlgItemText(hwnd, IDC_Estado1, "");
					SetDlgItemText(hwnd, IDC_Telefono, "");
					SetDlgItemText(hwnd, IDC_GrupoO, "");
					CheckRadioButton(hwnd, IDC_RHombre, IDC_RMujer, 0);
					SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)-1, 0);
					SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)-1, 0);
					SendDlgItemMessage(hwnd, IDC_Foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
					DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaNaci), GDT_NONE, 0);
				}

			}
		break;

		case IDC_BuscarIma: {
				OPENFILENAME ofn;
				ZeroMemory(&ofn, sizeof(ofn));

				ofn.lStructSize = sizeof(ofn);
				ofn.hwndOwner = hwnd;
				ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
				ofn.lpstrFile = zFile;
				ofn.lpstrFile[0] = '\0';
				ofn.nMaxFile = sizeof(zFile);
				ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
				ofn.nFilterIndex = 2;

				if (GetOpenFileName(&ofn) == TRUE) {
					HBITMAP bmp; //1
					imagenCargada = true;
					bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 200, 300, LR_LOADFROMFILE); //2
					SendDlgItemMessage(hwnd, IDC_Foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 

				}
				else {
					MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
				}
			}
		break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		}
		break;

		}
	}
	break;

	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
	break;

	}
	return false;
}
LRESULT CALLBACK AltaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		aux3 = inicio3;
		while (aux3) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_INSERTSTRING, (WPARAM)-1, (LPARAM)aux3->CURP);
			aux3 = aux3->sig;
		}

		aux2 = inicio2;
		while (aux2) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_INSERTSTRING, (WPARAM)-1, (LPARAM)aux2->ClaveVac);
			aux2 = aux2->sig;
		}

		char idStr[5];
		srand(time(NULL));
		do {
			int id = rand() % 9000 + 1000;
			sprintf_s(idStr, "%d", id);
		} while (IDExists(idStr));

		SetDlgItemText(hwnd, IDC_IDCarnet, idStr);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDOK: {
			bool bol = true;
			carnet* newCar = new carnet;

			char idStr[5];
			srand(time(NULL));
			do {
				int id = rand() % 9000 + 1000;
				sprintf_s(idStr, "%d", id);
			} while (IDExists(idStr));
			SetDlgItemText(hwnd, IDC_IDCarnet, idStr);

			GetDlgItemText(hwnd, IDC_IDCarnet, idStr, sizeof(idStr));
			strcpy_s(newCar->IDVac, sizeof(newCar->IDVac), idStr);
			GetDlgItemText(hwnd, IDC_LoteDeVac, newCar->Lote, sizeof(newCar->Lote));
			GetDlgItemText(hwnd, IDC_ModuloVac, newCar->CentroVac, sizeof(newCar->CentroVac));
			SYSTEMTIME fechaVac;
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), &fechaVac);
			newCar->FechaVac = fechaVac;

			char CURPSelect[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETCURSEL, 0, 0), (LPARAM)CURPSelect);
			strcpy_s(newCar->CURPPerson, sizeof(newCar->CURPPerson), CURPSelect);

			char ClaveVacSelect[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETCURSEL, 0, 0), (LPARAM)ClaveVacSelect);
			strcpy_s(newCar->ClaveVacCarnet, sizeof(newCar->ClaveVacCarnet), ClaveVacSelect);

			for (int i = 0; i < strlen(newCar->Lote); i++) {
				if (!isdigit(newCar->Lote[i]) && newCar->Lote[i] != ' ') {
					MessageBox(hwnd, "Lote invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			if (bol == true) {
				RegistrarCarnet(newCar);
				delete newCar;
				SetDlgItemText(hwnd, IDC_LoteDeVac, "");
				SetDlgItemText(hwnd, IDC_ModuloVac, "");
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SETCURSEL, (WPARAM)-1, 0);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SETCURSEL, (WPARAM)-1, 0);
				DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), GDT_NONE, 0);
			}

		}break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		}break;

		case IDC_MostrarInfoPac: {
			int index = SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETCURSEL, 0, 0);
			if (index == CB_ERR) {
				MessageBox(hwnd, "No ha seleccionado ningún elemento.", "Error", MB_OK | MB_ICONERROR);
				break;
			}
			char curp[20];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETLBTEXT, (WPARAM)index, (LPARAM)curp);
			aux3 = inicio3;
			while (aux3) {
				if (strncmp(aux3->CURP, curp, 18) == 0) {
					DialogBoxParam(hInstGlobal, MAKEINTRESOURCE(IDD_MostrarIndoCURP), hwnd, MostrarInfoPac, (LPARAM)aux3);
					break;
				}
				aux3 = aux3->sig;
			}
		}break;

		case IDC_MostrarInfoVac: {
			int index = SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETCURSEL, 0, 0);
			if (index == CB_ERR) {
				MessageBox(hwnd, "No ha seleccionado ningún elemento.", "Error", MB_OK | MB_ICONERROR);
				break;
			}
			char ClaveComp[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETLBTEXT, (WPARAM)index, (LPARAM)ClaveComp);
			aux2 = inicio2;
			while (aux2) {
				if (strcmp(aux2->ClaveVac, ClaveComp) == 0) {
					DialogBoxParam(hInstGlobal, MAKEINTRESOURCE(IDD_MostrarInfoVacuna), hwnd, MostrarInfoVac, (LPARAM)aux2);
					break;
				}
				aux2 = aux2->sig;
			}
		}break;

		}
	}break;

	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}break;

	}
	return false;
}

//Lista
LRESULT CALLBACK ListaVacunas(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		aux2 = inicio2;
		while (aux2 != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
			aux2 = aux2->sig;
		}
	}
	break;
	
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_LISTClaveVac: {
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {
				char textoList[50];
				int índice = 0;

				SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETTEXT, 3, (LPARAM)textoList);

				índice = SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETTEXT, índice, (LPARAM)textoList);
				aux2 = inicio2;
				while (aux2 && strcmp(textoList, aux2->ClaveVac) != 0) {
					aux2 = aux2->sig;
				}
				SetDlgItemText(hwnd, IDC_TipoVac, aux2->tipo);
				SetDlgItemText(hwnd, IDC_MarcaVac, aux2->marca);
				SetDlgItemText(hwnd, IDC_NoDosis, aux2->NDosis);
				SetDlgItemText(hwnd, IDC_ClaseVac, aux2->ClaveVac);
				SetDlgItemText(hwnd, IDC_Desc, aux2->Descrip);
				SetDlgItemText(hwnd, IDC_Precio, aux2->precio);

			}break;
			}
		}break;

		case IDC_Modificar: {
			char textoList[50];
			int indice = SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETCURSEL, 0, 0);
			if (indice == LB_ERR) {
				MessageBox(hwnd, "Por favor seleccione un elemento a modificar.", "Error", MB_ICONERROR | MB_OK);
				break;
			}
			EnableWindow(GetDlgItem(hwnd, IDC_TipoVac), true);
			EnableWindow(GetDlgItem(hwnd, IDC_MarcaVac), true);
			EnableWindow(GetDlgItem(hwnd, IDC_NoDosis), true);
			EnableWindow(GetDlgItem(hwnd, IDC_ClaseVac), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Desc), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Precio), true);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarVac), true);

		}break;

		case ID_GuardarVac: {
			bool bol = true;
			vacunas* nuevaInfo_Vac = new vacunas;
			char textoList[20];
			int índice = 0;
			índice = SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETTEXT, índice, (LPARAM)textoList);

			GetDlgItemText(hwnd, IDC_TipoVac, nuevaInfo_Vac->tipo, sizeof(nuevaInfo_Vac->tipo));
			GetDlgItemText(hwnd, IDC_MarcaVac, nuevaInfo_Vac->marca, sizeof(nuevaInfo_Vac->marca));
			GetDlgItemText(hwnd, IDC_NoDosis, nuevaInfo_Vac->NDosis, sizeof(nuevaInfo_Vac->NDosis));
			GetDlgItemText(hwnd, IDC_ClaseVac, nuevaInfo_Vac->ClaveVac, sizeof(nuevaInfo_Vac->ClaveVac));
			GetDlgItemText(hwnd, IDC_Desc, nuevaInfo_Vac->Descrip, sizeof(nuevaInfo_Vac->Descrip));
			GetDlgItemText(hwnd, IDC_Precio, nuevaInfo_Vac->precio, sizeof(nuevaInfo_Vac->precio));


			for (int i = 0; i < strlen(nuevaInfo_Vac->precio); i++) {
				if (!isdigit(nuevaInfo_Vac->precio[i]) && nuevaInfo_Vac->precio[i] != ' ') {
					MessageBox(hwnd, "Precio invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}
			for (int i = 0; i < strlen(nuevaInfo_Vac->NDosis); i++) {
				if (!isdigit(nuevaInfo_Vac->NDosis[i]) && nuevaInfo_Vac->NDosis[i] != ' ') {
					MessageBox(hwnd, "El numero de dosis debe ser en formato numerico", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			aux2 = inicio2;
			bool ClaveRepetida = false;
			while (aux2 != nullptr) {
				if (strcmp(aux2->ClaveVac, nuevaInfo_Vac->ClaveVac) == 0) {
					if (aux2 != nuevaInfo_Vac) { // Si no es el mismo nodo, mostrar la clave repetida
						ClaveRepetida = true;
						MessageBox(hwnd, "La clave de vacunación ya está en uso en otro registro.", "AVISO", MB_OK | MB_ICONERROR);
					}
					else { // Si es el mismo nodo, mostrar mensaje de clave duplicada
						ClaveRepetida = true;
						MessageBox(hwnd, "La clave de vacunación ya está en uso en este registro.", "AVISO", MB_OK | MB_ICONERROR);
					}
					break;
				}
				aux2 = aux2->sig;
			}

			if (strlen(nuevaInfo_Vac->precio) <= 0) {
				MessageBox(hwnd, "Solo se aceptan precios mayores a 0", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}

			if (bol == true && ClaveRepetida == false) {
				ModificarVacunas(nuevaInfo_Vac, textoList);
				delete nuevaInfo_Vac;

			}
			SetDlgItemText(hwnd, IDC_TipoVac, "");
			SetDlgItemText(hwnd, IDC_MarcaVac, "");
			SetDlgItemText(hwnd, IDC_NoDosis, "");
			SetDlgItemText(hwnd, IDC_ClaseVac, "");
			SetDlgItemText(hwnd, IDC_Desc, "");
			SetDlgItemText(hwnd, IDC_Precio, "");

			EnableWindow(GetDlgItem(hwnd, IDC_TipoVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_MarcaVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_NoDosis), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ClaseVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Desc), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Precio), false);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarVac), false);

			SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_RESETCONTENT, 0, 0);

			aux2 = inicio2;
			while (aux2 != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
				aux2 = aux2->sig;
			}


		}break;

		case IDC_Eliminar: {
			char textoList[50];
			int indice = SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETCURSEL, 0, 0);
			if (indice == LB_ERR) {
				MessageBox(hwnd, "Por favor seleccione un elemento a eliminar.", "Error", MB_ICONERROR | MB_OK);
				break;
			}

			SendDlgItemMessage(hwnd, IDC_LISTClaveVac, LB_GETTEXT, indice, (LPARAM)textoList);

			if (MessageBox(hwnd, "¿Está seguro de eliminar esta vacuna?", "Confirmar eliminación", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_DELETESTRING, (WPARAM)indice, 0);

				EliminarVac(textoList);

				aux2 = inicio2;
				while (aux2 != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
					SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)"--------------------");
					aux2 = aux2->sig;
				}
			}

			SetDlgItemText(hwnd, IDC_TipoVac, "");
			SetDlgItemText(hwnd, IDC_MarcaVac, "");
			SetDlgItemText(hwnd, IDC_NoDosis, "");
			SetDlgItemText(hwnd, IDC_ClaseVac, "");
			SetDlgItemText(hwnd, IDC_Desc, "");
			SetDlgItemText(hwnd, IDC_Precio, "");

			EnableWindow(GetDlgItem(hwnd, IDC_TipoVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_MarcaVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_NoDosis), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ClaseVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Desc), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Precio), false);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarVac), false);

			SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_RESETCONTENT, 0, 0);

			aux2 = inicio2;
			while (aux2 != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTClaveVac), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
				aux2 = aux2->sig;
			}

		}break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		}break;
		}
	}
				break;
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
				break;

	}
	return false;
}
LRESULT CALLBACK ListaPacientes(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {
		aux3 = inicio3;
		while (aux3 != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
			aux3 = aux3->sig;
		}
		char EstadoCivil[7][35] = { "1. Casado(a)", "2. Conviviente", "3. Anulado(a)", "4. Separado de unión legal","5. Separado(a) de unión de hecho", "6. Viudo(a)", "7. Soltero(a)" };
		for (int i = 0; i < 7; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOEstadoC, CB_INSERTSTRING, (WPARAM)i, (LPARAM)EstadoCivil[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)0, 0);

		char PerfilRiesg[3][10] = { "Alto", "Medio", "Bajo" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOPerfilDR, CB_INSERTSTRING, (WPARAM)i, (LPARAM)PerfilRiesg[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)0, 0);
	}
				break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDC_LISTPacientes: {
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {
				char textoList[50];
				int índice = 0;

				SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETTEXT, 3, (LPARAM)textoList);

				índice = SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETCURSEL, 0, 0);
				SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETTEXT, índice, (LPARAM)textoList);
				aux3 = inicio3;
				while (aux3 && strcmp(textoList, aux3->Nombres) != 0) {
					aux3 = aux3->sig;
				}

				SetDlgItemText(hwnd, IDC_ApellidoPP, aux3->ApellidoPP);
				SetDlgItemText(hwnd, IDC_ApellidoMP, aux3->ApellidoMP);
				SetDlgItemText(hwnd, IDC_NombresP, aux3->Nombres);
				SetDlgItemText(hwnd, IDC_CURP, aux3->CURP);
				SetDlgItemText(hwnd, IDC_RFC, aux3->RFC);
				SetDlgItemText(hwnd, IDC_Calle, aux3->Calle);
				SetDlgItemText(hwnd, IDC_Colonia, aux3->Colonia);
				SetDlgItemText(hwnd, IDC_Ciudad1, aux3->Ciudad);
				SetDlgItemText(hwnd, IDC_Estado1, aux3->Estado);
				SetDlgItemText(hwnd, IDC_Telefono, aux3->telefono);
				SetDlgItemText(hwnd, IDC_GrupoO, aux3->GrupoOcup);
				if (aux3->Sexo == 1) {
					CheckDlgButton(hwnd, IDC_RHombre, BST_CHECKED);
					CheckDlgButton(hwnd, IDC_RMujer, BST_UNCHECKED);
				}
				else if (aux3->Sexo == 2) {
					CheckDlgButton(hwnd, IDC_RHombre, BST_UNCHECKED);
					CheckDlgButton(hwnd, IDC_RMujer, BST_CHECKED);
				}
				
				SendDlgItemMessage(hwnd, IDC_FechaNaci, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&aux3->FechaNac);

				HBITMAP bmp;
				bmp = (HBITMAP)LoadImage(NULL, aux3->Doc, IMAGE_BITMAP, 200, 300, LR_LOADFROMFILE);
				SendDlgItemMessage(hwnd, IDC_Foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

				int EstadoCivilSelect = aux3->EstadoCiv - 1; // Obtener el índice del elemento que desea seleccionar
				SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, EstadoCivilSelect, 0); // Seleccionar el elemento deseado

				int PerfilRSelect = aux3->PerfilRies - 1; // Obtener el índice del elemento que desea seleccionar
				SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, PerfilRSelect, 0); // Seleccionar el elemento deseado

			}break;
		}
	} break;

		case IDC_ModificarPac: {
			char textoList[50];
			int indice = SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETCURSEL, 0, 0);
			if (indice == LB_ERR) {
				MessageBox(hwnd, "Por favor seleccione un elemento a modificar.", "Error", MB_ICONERROR | MB_OK);
				break;
			}
			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoPP), true);
			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoMP), true);
			EnableWindow(GetDlgItem(hwnd, IDC_NombresP), true);
			EnableWindow(GetDlgItem(hwnd, IDC_CURP), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RFC), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Calle), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Colonia), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Ciudad1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Estado1), true);
			EnableWindow(GetDlgItem(hwnd, IDC_Telefono), true);
			EnableWindow(GetDlgItem(hwnd, IDC_GrupoO), true);
			EnableWindow(GetDlgItem(hwnd, IDC_FechaNaci), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOEstadoC), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOPerfilDR), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RHombre), true);
			EnableWindow(GetDlgItem(hwnd, IDC_RMujer), true);
			EnableWindow(GetDlgItem(hwnd, IDC_BuscarIma), true);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarPaciente), true);

		} break;

		case ID_GuardarPaciente: {
			bool bol = true;
			personas* nuevaInfo_Pac = new personas;
			char textoListPac[20];
			int índice = 0;
			índice = SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETCURSEL, 0, 0);
			SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETTEXT, índice, (LPARAM)textoListPac);

			GetDlgItemText(hwnd, IDC_ApellidoPP, nuevaInfo_Pac->ApellidoPP, sizeof(nuevaInfo_Pac->ApellidoPP));
			GetDlgItemText(hwnd, IDC_ApellidoMP, nuevaInfo_Pac->ApellidoMP, sizeof(nuevaInfo_Pac->ApellidoMP));
			GetDlgItemText(hwnd, IDC_NombresP, nuevaInfo_Pac->Nombres, sizeof(nuevaInfo_Pac->Nombres));
			GetDlgItemText(hwnd, IDC_CURP, nuevaInfo_Pac->CURP, sizeof(nuevaInfo_Pac->CURP));
			GetDlgItemText(hwnd, IDC_RFC, nuevaInfo_Pac->RFC, sizeof(nuevaInfo_Pac->RFC));
			GetDlgItemText(hwnd, IDC_Calle, nuevaInfo_Pac->Calle, sizeof(nuevaInfo_Pac->Calle));
			GetDlgItemText(hwnd, IDC_Colonia, nuevaInfo_Pac->Colonia, sizeof(nuevaInfo_Pac->Colonia));
			GetDlgItemText(hwnd, IDC_Ciudad1, nuevaInfo_Pac->Ciudad, sizeof(nuevaInfo_Pac->Ciudad));
			GetDlgItemText(hwnd, IDC_Estado1, nuevaInfo_Pac->Estado, sizeof(nuevaInfo_Pac->Estado));
			GetDlgItemText(hwnd, IDC_Telefono, nuevaInfo_Pac->telefono, sizeof(nuevaInfo_Pac->telefono));
			GetDlgItemText(hwnd, IDC_GrupoO, nuevaInfo_Pac->GrupoOcup, sizeof(nuevaInfo_Pac->GrupoOcup));

			SYSTEMTIME fechaSeleccionada;
			ZeroMemory(&fechaSeleccionada, sizeof(fechaSeleccionada));
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_FechaNaci), &fechaSeleccionada);

			SYSTEMTIME fechaHoy;
			ZeroMemory(&fechaHoy, sizeof(fechaHoy));
			GetLocalTime(&fechaHoy);

			if (fechaSeleccionada.wYear > fechaHoy.wYear ||
				(fechaSeleccionada.wYear == fechaHoy.wYear && fechaSeleccionada.wMonth > fechaHoy.wMonth) ||
				(fechaSeleccionada.wYear == fechaHoy.wYear && fechaSeleccionada.wMonth == fechaHoy.wMonth && fechaSeleccionada.wDay > fechaHoy.wDay)) {
				MessageBox(hwnd, "Fecha invalida: no puedes seleccionar una fecha en el futuro", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}

			nuevaInfo_Pac->FechaNac = fechaSeleccionada;

			if (IsDlgButtonChecked(hwnd, IDC_RHombre) == BST_CHECKED) {
				nuevaInfo_Pac->Sexo = 1;
			}
			if (IsDlgButtonChecked(hwnd, IDC_RMujer) == BST_CHECKED) {
				nuevaInfo_Pac->Sexo = 2;
			}

			if (strlen(nuevaInfo_Pac->telefono) != 10) {
				MessageBox(hwnd, "Telefono invalido: requiere 10 digitos", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}
			for (int i = 0; i < strlen(nuevaInfo_Pac->telefono); i++) {
				if (!isdigit(nuevaInfo_Pac->telefono[i]) && nuevaInfo_Pac->telefono[i] != ' ') {
					MessageBox(hwnd, "Telefono invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
				}
			}

			int EstadoCivilSelect = 0;
			EstadoCivilSelect = SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_GETCURSEL, 0, 0);
			nuevaInfo_Pac->EstadoCiv = EstadoCivilSelect + 1;

			int PerfilRSelect = 0;
			PerfilRSelect = SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_GETCURSEL, 0, 0);
			nuevaInfo_Pac->PerfilRies = PerfilRSelect + 1;

			//validar curp y rfc
			if (strlen(nuevaInfo_Pac->CURP) != 18) {
				MessageBox(hwnd, "CURP invalido: requiere 18 elementos", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}
			if (strlen(nuevaInfo_Pac->RFC) != 13) {
				MessageBox(hwnd, "RFC invalido: requiere 13 elementos", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}

			aux3 = inicio3;
			bool CURPrepetido = false;
			while (aux3 != nullptr) {
				if (strcmp(aux3->CURP, nuevaInfo_Pac->CURP) == 0) {
					CURPrepetido = true;
					MessageBox(hwnd, "La CURP del paciente ya está registrada.", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
				aux3 = aux3->sig;
			}

			aux3 = inicio3;
			bool RFCrepetido = false;
			while (aux3 != nullptr) {
				if (strcmp(aux3->RFC, nuevaInfo_Pac->RFC) == 0) {
					RFCrepetido = true;
					MessageBox(hwnd, "El RFC del paciente ya está registrado.", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
				aux3 = aux3->sig;
			}

			/*if (!imagenCargada) {
				MessageBox(hwnd, "Cargue un documento valido", "AVISO", MB_OK | MB_ICONERROR);
				bol = false;
			}*/

			for (int i = 0; i < strlen(nuevaInfo_Pac->CURP); i++) {
				if (nuevaInfo_Pac->CURP[i] == ' ') {
					MessageBox(hwnd, "El CURP no puede contener espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}
			for (int i = 0; i < strlen(nuevaInfo_Pac->RFC); i++) {
				if (nuevaInfo_Pac->RFC[i] == ' ') {
					MessageBox(hwnd, "El RFC no puede contener espacios", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			strcpy_s(nuevaInfo_Pac->Doc, zFile);

			if (bol == true) {
				ModificarPacientes(nuevaInfo_Pac, textoListPac);
				delete nuevaInfo_Pac;
			}

			SetDlgItemText(hwnd, IDC_ApellidoPP, "");
			SetDlgItemText(hwnd, IDC_ApellidoMP, "");
			SetDlgItemText(hwnd, IDC_NombresP, "");
			SetDlgItemText(hwnd, IDC_CURP, "");
			SetDlgItemText(hwnd, IDC_RFC, "");
			SetDlgItemText(hwnd, IDC_Calle, "");
			SetDlgItemText(hwnd, IDC_Colonia, "");
			SetDlgItemText(hwnd, IDC_Ciudad1, "");
			SetDlgItemText(hwnd, IDC_Estado1, "");
			SetDlgItemText(hwnd, IDC_Telefono, "");
			SetDlgItemText(hwnd, IDC_GrupoO, "");
			CheckRadioButton(hwnd, IDC_RHombre, IDC_RMujer, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
			DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaNaci), GDT_NONE, 0);

			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoPP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoMP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_NombresP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_CURP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RFC), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Calle), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Colonia), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Ciudad1), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Estado1), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Telefono), false);
			EnableWindow(GetDlgItem(hwnd, IDC_GrupoO), false);
			EnableWindow(GetDlgItem(hwnd, IDC_FechaNaci), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOEstadoC), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOPerfilDR), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RHombre), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RMujer), false);
			EnableWindow(GetDlgItem(hwnd, IDC_BuscarIma), false);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarPaciente), false);

			SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_RESETCONTENT, 0, 0);

			aux3 = inicio3;
			while (aux3 != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
				aux3 = aux3->sig;
			}



		} break;

		case IDC_BuscarIma: {
			OPENFILENAME ofn;
			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hwnd;
			ofn.lpstrFilter = "ALL\0 * .*\0Bitmaps\0 * .bmp\0";
			ofn.lpstrFile = zFile;
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(zFile);
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
			ofn.nFilterIndex = 2;

			if (GetOpenFileName(&ofn) == TRUE) {
				HBITMAP bmp; //1
				imagenCargada = true;
				bmp = (HBITMAP)LoadImage(NULL, zFile, IMAGE_BITMAP, 200, 300, LR_LOADFROMFILE); //2
				SendDlgItemMessage(hwnd, IDC_Foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp); //3 

			}
			else {
				MessageBox(hwnd, "No seleccionó foto", "AVISO", MB_OK | MB_ICONERROR);
			}
		} break;

		case IDC_Eliminar: {
			char textoList[50];
			int indice = SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETCURSEL, 0, 0);
			if (indice == LB_ERR) {
				MessageBox(hwnd, "Por favor seleccione un elemento a eliminar.", "Error", MB_ICONERROR | MB_OK);
				break;
			}

			SendDlgItemMessage(hwnd, IDC_LISTPacientes, LB_GETTEXT, indice, (LPARAM)textoList);

			if (MessageBox(hwnd, "¿Está seguro de eliminar esta persona?", "Confirmar eliminación", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_DELETESTRING, (WPARAM)indice, 0);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_DELETESTRING, (WPARAM)indice, 0);

				EliminarPac(textoList);

				aux2 = inicio2;
				while (aux2 != nullptr) {
					SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux2->ClaveVac);
					SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)"--------------------");
					aux2 = aux2->sig;
				}
			}

			SetDlgItemText(hwnd, IDC_ApellidoPP, "");
			SetDlgItemText(hwnd, IDC_ApellidoMP, "");
			SetDlgItemText(hwnd, IDC_NombresP, "");
			SetDlgItemText(hwnd, IDC_CURP, "");
			SetDlgItemText(hwnd, IDC_RFC, "");
			SetDlgItemText(hwnd, IDC_Calle, "");
			SetDlgItemText(hwnd, IDC_Colonia, "");
			SetDlgItemText(hwnd, IDC_Ciudad1, "");
			SetDlgItemText(hwnd, IDC_Estado1, "");
			SetDlgItemText(hwnd, IDC_Telefono, "");
			SetDlgItemText(hwnd, IDC_GrupoO, "");
			CheckRadioButton(hwnd, IDC_RHombre, IDC_RMujer, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_Foto), STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)NULL);
			DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaNaci), GDT_NONE, 0);

			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoPP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ApellidoMP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_NombresP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_CURP), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RFC), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Calle), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Colonia), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Ciudad1), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Estado1), false);
			EnableWindow(GetDlgItem(hwnd, IDC_Telefono), false);
			EnableWindow(GetDlgItem(hwnd, IDC_GrupoO), false);
			EnableWindow(GetDlgItem(hwnd, IDC_FechaNaci), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOEstadoC), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOPerfilDR), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RHombre), false);
			EnableWindow(GetDlgItem(hwnd, IDC_RMujer), false);
			EnableWindow(GetDlgItem(hwnd, IDC_BuscarIma), false);
			EnableWindow(GetDlgItem(hwnd, ID_GuardarPaciente), false);

			SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_RESETCONTENT, 0, 0);
			SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_RESETCONTENT, 0, 0);

			aux3 = inicio3;
			while (aux3 != nullptr) {
				SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
				SendMessage(GetDlgItem(hPrinGlob, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
				aux3 = aux3->sig;
			}

		} break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		} break;

		}
	}
				break;
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
				break;

	}
	return false;
}
LRESULT CALLBACK ListaCarnet(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	bool ordenado = false;
	switch (msg) {
	case WM_INITDIALOG: {
		aux4 = inicio4;
		while (aux4) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_INSERTSTRING, (WPARAM)-1, (LPARAM)aux4->CURPPerson);
			aux4 = aux4->sig;
		}

		aux3 = inicio3;
		while (aux3) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_INSERTSTRING, (WPARAM)-1, (LPARAM)aux3->CURP);
			aux3 = aux3->sig;
		}

		aux2 = inicio2;
		while (aux2) {
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_INSERTSTRING, (WPARAM)-1, (LPARAM)aux2->ClaveVac);
			aux2 = aux2->sig;
		}
		
		aux4 = inicio4;
		while (aux4 != nullptr) {
			char textoList[100];
			sprintf_s(textoList, sizeof(textoList), "%s - %s", aux4->IDVac, aux4->CURPPerson);
			SendMessage(GetDlgItem(hwnd, IDC_LISTMostrarCarnet), LB_ADDSTRING, 0, (LPARAM)textoList);
			aux4 = aux4->sig;
		}

	}
				break;
	case WM_COMMAND: {
		switch (LOWORD(wParam)) {

		case IDC_LISTMostrarCarnet: {
			switch (HIWORD(wParam)) {
			case LBN_SELCHANGE: {
				char textoList[50];
				int índice = SendDlgItemMessage(hwnd, IDC_LISTMostrarCarnet, LB_GETCURSEL, 0, 0);
				if (índice != LB_ERR) {
					SendDlgItemMessage(hwnd, IDC_LISTMostrarCarnet, LB_GETTEXT, índice, (LPARAM)textoList);
					aux4 = inicio4;
					while (aux4 != nullptr && strcmp((std::string(aux4->CURPPerson) + " - " + std::string(aux4->IDVac)).c_str(), textoList) != 0 && strcmp((std::string(aux4->IDVac) + " - " + std::string(aux4->CURPPerson)).c_str(), textoList) != 0) {
						aux4 = aux4->sig;
					}

					if (aux4 != nullptr) {
						SetDlgItemText(hwnd, IDC_LoteDeVac, aux4->Lote);
						SetDlgItemText(hwnd, IDC_IDCarnet, aux4->IDVac);
						SetDlgItemText(hwnd, IDC_ModuloVac, aux4->CentroVac);
						SendDlgItemMessage(hwnd, IDC_FechaVacu, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&aux4->FechaVac);
						SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->CURPPerson);
						SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->ClaveVacCarnet);
					}
				}
			} break;
			}
		} break;

		case IDC_BOrdenarID: {
			if (!ordenado) { // Si no ha sido ordenado aún
				// Actualizamos el ListBox con los elementos ordenados
				AgregarElementosListBox(hwnd, inicio4, true);

				ordenado = true; // Marcamos como ordenado

				// Marcamos el botón como presionado
				SendMessage(GetDlgItem(hwnd, IDC_BOrdenarID), BM_SETCHECK, BST_CHECKED, 0);
			}
		} break;
		
		case IDC_BOrdenarCURP: {
			bool ordenar = SendMessage(GetDlgItem(hwnd, IDC_BOrdenarCURP), BM_GETCHECK, 0, 0) == BST_CHECKED;

			AgregarElementosListBoxCURP(hwnd, inicio4, ordenar);

			SendMessage(GetDlgItem(hwnd, IDC_BOrdenarCURP), BM_SETCHECK, ordenar ? BST_CHECKED : BST_UNCHECKED, 0);

			ReporteQuickCSV(hwnd, inicio4, true);

		} break;

		case IDC_COMBOCURPCar: {
			if (HIWORD(wParam) == CBN_SELCHANGE) {
				int index = SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0);
				if (index != CB_ERR) {
					char curp[20];
					SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETLBTEXT, (WPARAM)index, (LPARAM)curp);

					aux4 = inicio4;
					while (aux4) {
						if (strcmp(aux4->CURPPerson, curp) == 0) {
							SetDlgItemText(hwnd, IDC_LoteDeVac, aux4->Lote);
							SetDlgItemText(hwnd, IDC_IDCarnet, aux4->IDVac);
							SetDlgItemText(hwnd, IDC_ModuloVac, aux4->CentroVac);
							SendDlgItemMessage(hwnd, IDC_FechaVacu, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&aux4->FechaVac);
							SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->CURPPerson);
							SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->ClaveVacCarnet);
							break;
						}
						aux4 = aux4->sig;
					}
				}
			}
		}break;

		case IDC_BuscarBina: {
			// Obtener el ID a buscar del edit control
			char idBuscado[10];
			GetDlgItemText(hwnd, IDC_IDCARMOSTRAR, idBuscado, sizeof(idBuscado));

			// Realizar la búsqueda binaria
			carnet* aux4 = buscarPorID(inicio4, fin4, idBuscado);

			// Si se encontró, mostrar la información en los controles
			if (aux4 != nullptr) {
				SetDlgItemText(hwnd, IDC_LoteDeVac, aux4->Lote);
				SetDlgItemText(hwnd, IDC_IDCarnet, aux4->IDVac);
				SetDlgItemText(hwnd, IDC_ModuloVac, aux4->CentroVac);
				SendDlgItemMessage(hwnd, IDC_FechaVacu, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&aux4->FechaVac);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->CURPPerson);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SELECTSTRING, (WPARAM)-1, (LPARAM)aux4->ClaveVacCarnet);
			}
			else {
				MessageBox(hwnd, "ID no encontrado", "Error", MB_OK | MB_ICONERROR);
				SetDlgItemText(hwnd, IDC_LoteDeVac, "");
				SetDlgItemText(hwnd, IDC_IDCarnet, "");
				SetDlgItemText(hwnd, IDC_ModuloVac, "");
				DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), GDT_NONE, 0);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SETCURSEL, (WPARAM)-1, 0);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SETCURSEL, (WPARAM)-1, 0);
				SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_SETCURSEL, (WPARAM)-1, 0);
			}
		} break;

		case IDC_ModificarCar: {
			char textoList[50];
			int indice = SendDlgItemMessage(hwnd, IDC_LISTMostrarCarnet, LB_GETCURSEL, 0, 0);
			bool found = false;
			if (SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0) != CB_ERR ||
				GetWindowTextLength(GetDlgItem(hwnd, IDC_IDCARMOSTRAR)) > 0 || indice != LB_ERR) {
				found = true;
			}

			if (found) {
				EnableWindow(GetDlgItem(hwnd, IDC_BuscarBina), false);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBOCURPCar), false);
				EnableWindow(GetDlgItem(hwnd, IDC_IDCARMOSTRAR), false);
				EnableWindow(GetDlgItem(hwnd, IDC_LoteDeVac), true);
				EnableWindow(GetDlgItem(hwnd, IDC_ModuloVac), true);
				EnableWindow(GetDlgItem(hwnd, IDC_FechaVacu), true);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), true);
				EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), true);
				EnableWindow(GetDlgItem(hwnd, IDOK), true);
			}
			else {
				MessageBox(hwnd, "Seleccione un elemento o busque por ID para modificar.", "No hay elementos para modificar", MB_OK | MB_ICONINFORMATION);
			}
		}break;

		case IDOK: {
			char textoListCar[50];

			if (SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0) != CB_ERR) {
				SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0), (LPARAM)textoListCar);
			}
			else {
				GetDlgItemText(hwnd, IDC_IDCARMOSTRAR, textoListCar, sizeof(textoListCar));
			}

			bool bol = true;
			carnet* nuevaInfo_Car = new carnet;
			char idStr[5];

			GetDlgItemText(hwnd, IDC_LoteDeVac, nuevaInfo_Car->Lote, sizeof(nuevaInfo_Car->Lote));
			GetDlgItemText(hwnd, IDC_ModuloVac, nuevaInfo_Car->CentroVac, sizeof(nuevaInfo_Car->CentroVac));
			SYSTEMTIME fechaVac;
			DateTime_GetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), &fechaVac);
			nuevaInfo_Car->FechaVac = fechaVac;

			char CURPSelect[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETCURSEL, 0, 0), (LPARAM)CURPSelect);
			strcpy_s(nuevaInfo_Car->CURPPerson, sizeof(nuevaInfo_Car->CURPPerson), CURPSelect);

			char ClaveVacSelect[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETCURSEL, 0, 0), (LPARAM)ClaveVacSelect);
			strcpy_s(nuevaInfo_Car->ClaveVacCarnet, sizeof(nuevaInfo_Car->ClaveVacCarnet), ClaveVacSelect);

			for (int i = 0; i < strlen(nuevaInfo_Car->Lote); i++) {
				if (!isdigit(nuevaInfo_Car->Lote[i]) && nuevaInfo_Car->Lote[i] != ' ') {
					MessageBox(hwnd, "Lote invalido, solo aceptan caracteres numericos", "AVISO", MB_OK | MB_ICONERROR);
					bol = false;
					break;
				}
			}

			if (bol == true) {
				ModificarCarnet(nuevaInfo_Car, textoListCar);
				delete nuevaInfo_Car;
			}
			SetDlgItemText(hwnd, IDC_IDCarnet, "");
			SetDlgItemText(hwnd, IDC_LoteDeVac, "");
			SetDlgItemText(hwnd, IDC_ModuloVac, "");
			SetDlgItemText(hwnd, IDC_IDCarnet, "");
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SETCURSEL, (WPARAM)-1, 0);
			DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), GDT_NONE, 0);

			EnableWindow(GetDlgItem(hwnd, IDC_BuscarBina), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOCURPCar), true);
			EnableWindow(GetDlgItem(hwnd, IDC_IDCARMOSTRAR), true);
			EnableWindow(GetDlgItem(hwnd, IDC_LoteDeVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ModuloVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_FechaVacu), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), false);
			EnableWindow(GetDlgItem(hwnd, IDOK), false);

		}break;

		case IDC_EliminarCar: {
			int indice = SendDlgItemMessage(hwnd, IDC_LISTMostrarCarnet, LB_GETCURSEL, 0, 0);
			bool found = false;
			if (SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0) != CB_ERR || GetWindowTextLength(GetDlgItem(hwnd, IDC_IDCARMOSTRAR)) > 0||indice != LB_ERR) {
				found = true;
			}
			if (!found) {
				MessageBox(hwnd, "Seleccione un elemento o busque por ID para eliminar.", "No hay elementos para eliminar", MB_OK | MB_ICONINFORMATION);
				return 0;
			}

			char textoListCar[50];

			if (SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0) != CB_ERR) {
				SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETLBTEXT, SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_GETCURSEL, 0, 0), (LPARAM)textoListCar);
			}
			else {
				GetDlgItemText(hwnd, IDC_IDCARMOSTRAR, textoListCar, sizeof(textoListCar));
			}

			if (textoListCar[0] == '\0') {
				MessageBox(hwnd, "Ingrese el carnet que desea eliminar.", "Carnet vacío", MB_OK | MB_ICONINFORMATION);
				return 0;
			}


			if (MessageBox(hwnd, "¿Está seguro de eliminar este carnet?", "Confirmar eliminación", MB_YESNO | MB_ICONQUESTION) == IDYES) {
				EliminarCar(textoListCar);

				aux4 = inicio4;
				while (aux4 != nullptr) {
					SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_ADDSTRING, 0, (LPARAM)aux4->CURPPerson);
					aux4 = aux4->sig;
				}
				SendMessage(GetDlgItem(hwnd, IDC_COMBOCURPCar), CB_RESETCONTENT, 0, 0);

				// Limpiar el control de edición
				SetDlgItemText(hwnd, IDC_IDCARMOSTRAR, "");
				

			}

			SetDlgItemText(hwnd, IDC_IDCarnet, "");
			SetDlgItemText(hwnd, IDC_LoteDeVac, "");
			SetDlgItemText(hwnd, IDC_ModuloVac, "");
			SetDlgItemText(hwnd, IDC_IDCarnet, "");
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_SETCURSEL, (WPARAM)-1, 0);
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_SETCURSEL, (WPARAM)-1, 0);
			DateTime_SetSystemtime(GetDlgItem(hwnd, IDC_FechaVacu), GDT_NONE, 0);

			EnableWindow(GetDlgItem(hwnd, IDC_BuscarBina), true);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOCURPCar), true);
			EnableWindow(GetDlgItem(hwnd, IDC_IDCARMOSTRAR), true);
			EnableWindow(GetDlgItem(hwnd, IDC_LoteDeVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_ModuloVac), false);
			EnableWindow(GetDlgItem(hwnd, IDC_FechaVacu), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), false);
			EnableWindow(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), false);
			EnableWindow(GetDlgItem(hwnd, IDOK), false);

		} break;

		case IDC_MostrarInfoPac: {
			int index = SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETCURSEL, 0, 0);
			if (index == CB_ERR) {
				MessageBox(hwnd, "No ha seleccionado ningún elemento.", "Error", MB_OK | MB_ICONERROR);
				break;
			}
			char curp[20];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarCurp), CB_GETLBTEXT, (WPARAM)index, (LPARAM)curp);
			aux3 = inicio3;
			while (aux3) {
				if (strncmp(aux3->CURP, curp, 18) == 0) {
					DialogBoxParam(hInstGlobal, MAKEINTRESOURCE(IDD_MostrarIndoCURP), hwnd, MostrarInfoPac, (LPARAM)aux3);
					break;
				}
				aux3 = aux3->sig;
			}
		}break;

		case IDC_MostrarInfoVac: {
			int index = SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETCURSEL, 0, 0);
			if (index == CB_ERR) {
				MessageBox(hwnd, "No ha seleccionado ningún elemento.", "Error", MB_OK | MB_ICONERROR);
				break;
			}
			char ClaveComp[50];
			SendMessage(GetDlgItem(hwnd, IDC_COMBOMostrarClaveVac), CB_GETLBTEXT, (WPARAM)index, (LPARAM)ClaveComp);
			aux2 = inicio2;
			while (aux2) {
				if (strcmp(aux2->ClaveVac, ClaveComp) == 0) {
					DialogBoxParam(hInstGlobal, MAKEINTRESOURCE(IDD_MostrarInfoVacuna), hwnd, MostrarInfoVac, (LPARAM)aux2);
					break;
				}
				aux2 = aux2->sig;
			}
		}break;

		case IDCANCEL: {
			EndDialog(hwnd, 0);
		} break;

		}
	}
				break;
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
				break;
	}
	return false;
}

//Mostrar Pacientes
LRESULT CALLBACK MostrarInfoPac(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDCANCEL: {
			EndDialog(hwnd, 0);
		} break;
		}
	}
				break;
	case WM_INITDIALOG: {
		aux3 = inicio3;
		while (aux3 != nullptr) {
			SendMessage(GetDlgItem(hwnd, IDC_LISTPacientes), LB_ADDSTRING, 0, (LPARAM)aux3->Nombres);
			aux3 = aux3->sig;
		}
		char EstadoCivil[7][35] = { "1. Casado(a)", "2. Conviviente", "3. Anulado(a)", "4. Separado de unión legal","5. Separado(a) de unión de hecho", "6. Viudo(a)", "7. Soltero(a)" };
		for (int i = 0; i < 7; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOEstadoC, CB_INSERTSTRING, (WPARAM)i, (LPARAM)EstadoCivil[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, (WPARAM)0, 0);

		char PerfilRiesg[3][10] = { "Alto", "Medio", "Bajo" };
		for (int i = 0; i < 3; i++) {
			SendDlgItemMessage(hwnd, IDC_COMBOPerfilDR, CB_INSERTSTRING, (WPARAM)i, (LPARAM)PerfilRiesg[i]);
		}
		SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, (WPARAM)0, 0);

		personas* aux3 = (personas*)lParam;

		SetDlgItemText(hwnd, IDC_ApellidoPP, aux3->ApellidoPP);
		SetDlgItemText(hwnd, IDC_ApellidoMP, aux3->ApellidoMP);
		SetDlgItemText(hwnd, IDC_NombresP, aux3->Nombres);
		SetDlgItemText(hwnd, IDC_CURP, aux3->CURP);
		SetDlgItemText(hwnd, IDC_RFC, aux3->RFC);
		SetDlgItemText(hwnd, IDC_Calle, aux3->Calle);
		SetDlgItemText(hwnd, IDC_Colonia, aux3->Colonia);
		SetDlgItemText(hwnd, IDC_Ciudad1, aux3->Ciudad);
		SetDlgItemText(hwnd, IDC_Estado1, aux3->Estado);
		SetDlgItemText(hwnd, IDC_Telefono, aux3->telefono);
		SetDlgItemText(hwnd, IDC_GrupoO, aux3->GrupoOcup);
		if (aux3->Sexo == 1) {
			CheckDlgButton(hwnd, IDC_RHombre, BST_CHECKED);
			CheckDlgButton(hwnd, IDC_RMujer, BST_UNCHECKED);
		}
		else if (aux3->Sexo == 2) {
			CheckDlgButton(hwnd, IDC_RHombre, BST_UNCHECKED);
			CheckDlgButton(hwnd, IDC_RMujer, BST_CHECKED);
		}

		SendDlgItemMessage(hwnd, IDC_FechaNaci, DTM_SETSYSTEMTIME, (WPARAM)GDT_VALID, (LPARAM)&aux3->FechaNac);

		HBITMAP bmp;
		bmp = (HBITMAP)LoadImage(NULL, aux3->Doc, IMAGE_BITMAP, 200, 300, LR_LOADFROMFILE);
		SendDlgItemMessage(hwnd, IDC_Foto, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)bmp);

		int EstadoCivilSelect = aux3->EstadoCiv - 1; // Obtener el índice del elemento que desea seleccionar
		SendMessage(GetDlgItem(hwnd, IDC_COMBOEstadoC), CB_SETCURSEL, EstadoCivilSelect, 0); // Seleccionar el elemento deseado

		int PerfilRSelect = aux3->PerfilRies - 1; // Obtener el índice del elemento que desea seleccionar
		SendMessage(GetDlgItem(hwnd, IDC_COMBOPerfilDR), CB_SETCURSEL, PerfilRSelect, 0); // Seleccionar el elemento deseado

	}
				break;
	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}
				break;
	}
	return false;
}
LRESULT CALLBACK MostrarInfoVac(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_INITDIALOG: {

		SetDlgItemText(hwnd, IDC_TipoVac, aux2->tipo);
		SetDlgItemText(hwnd, IDC_MarcaVac, aux2->marca);
		SetDlgItemText(hwnd, IDC_NoDosis, aux2->NDosis);
		SetDlgItemText(hwnd, IDC_ClaseVac, aux2->ClaveVac);
		SetDlgItemText(hwnd, IDC_Desc, aux2->Descrip);
		SetDlgItemText(hwnd, IDC_Precio, aux2->precio);

	}break;

	case WM_COMMAND: {
		switch (LOWORD(wParam)) {
		case IDCANCEL: {
			EndDialog(hwnd, 0);
		} break;
		}
	}break;

	case WM_CLOSE: {
		EndDialog(hwnd, 0);
	}break;

	}
	return false;
}

//Registrar
void RegistrarUsuario(usuario* nuevo) {
	if (inicio1 == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicio1 = new usuario;
		aux1 = inicio1;

		aux1->sig = nullptr;
		aux1->ant = nullptr;

		strcpy_s(aux1->nomUsu, nuevo->nomUsu);
		strcpy_s(aux1->ApellidoP, nuevo->ApellidoP);
		strcpy_s(aux1->contra, nuevo->contra);
		strcpy_s(aux1->ApellidoM, nuevo->ApellidoM);
		strcpy_s(aux1->clave, nuevo->clave);

	}
	else {
		aux1 = inicio1;

		while (aux1->sig != nullptr)
		{
			aux1 = aux1->sig;
		}

		aux1->sig = new usuario;

		aux1->sig->sig = nullptr;
		aux1->sig->ant = aux1;

		aux1 = aux1->sig;

		strcpy_s(aux1->nomUsu, nuevo->nomUsu);
		strcpy_s(aux1->ApellidoP, nuevo->ApellidoP);
		strcpy_s(aux1->contra, nuevo->contra);
		strcpy_s(aux1->ApellidoM, nuevo->ApellidoM);
		strcpy_s(aux1->clave, nuevo->clave);

	}
	MessageBox(0, "Usuario registrado", "AVISO", MB_OK);
}
void RegistrarVacunas(vacunas* nuevo) {
	if (inicio2 == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicio2 = new vacunas;
		aux2 = inicio2;

		aux2->sig = nullptr;
		aux2->ant = nullptr;

		strcpy_s(aux2->tipo, nuevo->tipo);
		strcpy_s(aux2->marca, nuevo->marca);
		strcpy_s(aux2->NDosis, nuevo->NDosis);
		strcpy_s(aux2->ClaveVac, nuevo->ClaveVac);
		strcpy_s(aux2->Descrip, nuevo->Descrip);
		strcpy_s(aux2->precio, nuevo->precio);

	}
	else {
		aux2 = inicio2;

		while (aux2->sig != nullptr)
		{
			aux2 = aux2->sig;
		}

		aux2->sig = new vacunas;

		aux2->sig->sig = nullptr;
		aux2->sig->ant = aux2;

		aux2 = aux2->sig;

		strcpy_s(aux2->tipo, nuevo->tipo);
		strcpy_s(aux2->marca, nuevo->marca);
		strcpy_s(aux2->NDosis, nuevo->NDosis);
		strcpy_s(aux2->ClaveVac, nuevo->ClaveVac);
		strcpy_s(aux2->Descrip, nuevo->Descrip);
		strcpy_s(aux2->precio, nuevo->precio);

	}
	MessageBox(0, "Vacuna registrada", "AVISO", MB_OK);
}
void RegistrarPersonas(personas* nuevo) {
	if (inicio3 == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicio3 = new personas;
		aux3 = inicio3;

		aux3->sig = nullptr;
		aux3->ant = nullptr;

		strcpy_s(aux3->ApellidoPP, nuevo->ApellidoPP);
		strcpy_s(aux3->ApellidoMP, nuevo->ApellidoMP);
		strcpy_s(aux3->Nombres, nuevo->Nombres);
		strcpy_s(aux3->CURP, nuevo->CURP);
		strcpy_s(aux3->RFC, nuevo->RFC);
		strcpy_s(aux3->Calle, nuevo->Calle);
		strcpy_s(aux3->Colonia, nuevo->Colonia);
		strcpy_s(aux3->Ciudad, nuevo->Ciudad);
		strcpy_s(aux3->Estado, nuevo->Estado);
		strcpy_s(aux3->telefono, nuevo->telefono);
		strcpy_s(aux3->GrupoOcup, nuevo->GrupoOcup);
		aux3->EstadoCiv = nuevo->EstadoCiv;
		aux3->PerfilRies = nuevo->PerfilRies;
		aux3->Sexo = nuevo->Sexo;
		memcpy(&aux3->FechaNac, &nuevo->FechaNac, sizeof(SYSTEMTIME));
		strcpy_s(aux3->Doc, nuevo->Doc);
	}
	else {
		aux3 = inicio3;

		while (aux3->sig != nullptr)
		{
			aux3 = aux3->sig;
		}

		aux3->sig = new personas;

		aux3->sig->sig = nullptr;
		aux3->sig->ant = aux3;

		aux3 = aux3->sig;

		strcpy_s(aux3->ApellidoPP, nuevo->ApellidoPP);
		strcpy_s(aux3->ApellidoMP, nuevo->ApellidoMP);
		strcpy_s(aux3->Nombres, nuevo->Nombres);
		strcpy_s(aux3->CURP, nuevo->CURP);
		strcpy_s(aux3->RFC, nuevo->RFC);
		strcpy_s(aux3->Calle, nuevo->Calle);
		strcpy_s(aux3->Colonia, nuevo->Colonia);
		strcpy_s(aux3->Ciudad, nuevo->Ciudad);
		strcpy_s(aux3->Estado, nuevo->Estado);
		strcpy_s(aux3->telefono, nuevo->telefono);
		strcpy_s(aux3->GrupoOcup, nuevo->GrupoOcup);
		aux3->EstadoCiv = nuevo->EstadoCiv;
		aux3->PerfilRies = nuevo->PerfilRies;
		aux3->Sexo = nuevo->Sexo;
		memcpy(&aux3->FechaNac, &nuevo->FechaNac, sizeof(SYSTEMTIME));
		strcpy_s(aux3->Doc, nuevo->Doc);

	}
	MessageBox(0, "Paciente registrado", "AVISO", MB_OK);
}
void RegistrarCarnet(carnet* nuevo) {
	if (inicio4 == nullptr) { //Si 'inicio es igual a nullptr, o sea, apunta a nada, la lista esta vacia
		inicio4 = new carnet;
		aux4 = inicio4;

		aux4->sig = nullptr;
		aux4->ant = nullptr;
		
		memcpy(&aux4->FechaVac, &nuevo->FechaVac, sizeof(SYSTEMTIME));
		strcpy_s(aux4->Lote, nuevo->Lote);
		strcpy_s(aux4->CentroVac, nuevo->CentroVac);
		strcpy_s(aux4->IDVac, nuevo->IDVac);
		strcpy_s(aux4->CURPPerson, nuevo->CURPPerson);
		strcpy_s(aux4->ClaveVacCarnet, nuevo->ClaveVacCarnet);
		fin4 = aux4;
	}
	else {
		aux4 = inicio4;

		while (aux4->sig != nullptr)
		{
			aux4 = aux4->sig;
		}

		aux4->sig = new carnet;

		aux4->sig->sig = nullptr;
		aux4->sig->ant = aux4;

		aux4 = aux4->sig;

		memcpy(&aux4->FechaVac, &nuevo->FechaVac, sizeof(SYSTEMTIME));
		strcpy_s(aux4->Lote, nuevo->Lote);
		strcpy_s(aux4->CentroVac, nuevo->CentroVac);
		strcpy_s(aux4->IDVac, nuevo->IDVac);
		strcpy_s(aux4->CURPPerson, nuevo->CURPPerson);
		strcpy_s(aux4->ClaveVacCarnet, nuevo->ClaveVacCarnet);
		fin4 = aux4;
	}
	MessageBox(0, "Carnet registrado", "AVISO", MB_OK);
}

//Verificaciones
bool IDExists(const char* id) {
	carnet* temp = inicio4;
	while (temp != nullptr) {
		if (strcmp(temp->IDVac, id) == 0) {
			return true;
		}
		temp = temp->sig;
	}
	return false;
}

//Modificar
void ModificarVacunas(vacunas* nuevaInfo_Vac, char* textoList) {
	vacunas* aux2 = inicio2;
	while (aux2 != nullptr) {
		if (strcmp(aux2->ClaveVac, textoList) == 0) {
			// Actualizar información del nodo
			strcpy_s(aux2->tipo, nuevaInfo_Vac->tipo);
			strcpy_s(aux2->marca, nuevaInfo_Vac->marca);
			strcpy_s(aux2->NDosis, nuevaInfo_Vac->NDosis);
			strcpy_s(aux2->ClaveVac, nuevaInfo_Vac->ClaveVac);
			strcpy_s(aux2->Descrip, nuevaInfo_Vac->Descrip);
			strcpy_s(aux2->precio, nuevaInfo_Vac->precio);
			break;
		}
		aux2 = aux2->sig;
	}
	MessageBox(NULL, "La información de la vacuna ha sido modificada con éxito.", "Modificar Vacuna", MB_OK | MB_ICONINFORMATION);
}
void ModificarPacientes(personas* nuevaInfo_Pac, char* textoListPac) {
	personas* aux3 = inicio3;
	while (aux3 != nullptr) {
		if (strcmp(aux3->Nombres, textoListPac) == 0) {
			// Actualizar información del nodo
			strcpy_s(aux3->ApellidoPP, nuevaInfo_Pac->ApellidoPP);
			strcpy_s(aux3->ApellidoMP, nuevaInfo_Pac->ApellidoMP);
			strcpy_s(aux3->Nombres, nuevaInfo_Pac->Nombres);
			strcpy_s(aux3->CURP, nuevaInfo_Pac->CURP);
			strcpy_s(aux3->RFC, nuevaInfo_Pac->RFC);
			strcpy_s(aux3->Calle, nuevaInfo_Pac->Calle);
			strcpy_s(aux3->Colonia, nuevaInfo_Pac->Colonia);
			strcpy_s(aux3->Ciudad, nuevaInfo_Pac->Ciudad);
			strcpy_s(aux3->Estado, nuevaInfo_Pac->Estado);
			strcpy_s(aux3->telefono, nuevaInfo_Pac->telefono);
			strcpy_s(aux3->GrupoOcup, nuevaInfo_Pac->GrupoOcup);
			aux3->EstadoCiv = nuevaInfo_Pac->EstadoCiv;
			aux3->PerfilRies = nuevaInfo_Pac->PerfilRies;
			aux3->Sexo = nuevaInfo_Pac->Sexo;
			memcpy(&aux3->FechaNac, &nuevaInfo_Pac->FechaNac, sizeof(SYSTEMTIME));
			strcpy_s(aux3->Doc, nuevaInfo_Pac->Doc);
			break;
		}
		aux3 = aux3->sig;
	}
	MessageBox(NULL, "La información de la persona ha sido modificada con éxito.", "Modificar Persona", MB_OK | MB_ICONINFORMATION);
}
void ModificarCarnet(carnet* nuevaInfo_Car, char* textoListCar) {
	carnet* aux4 = inicio4;
	while (aux4 != nullptr) {
		if (strcmp(aux4->CURPPerson, textoListCar) == 0 || strcmp(aux4->ClaveVacCarnet, textoListCar) == 0) {
			// Actualizar información del nodo
			memcpy(&aux4->FechaVac, &nuevaInfo_Car->FechaVac, sizeof(SYSTEMTIME));
			strcpy_s(aux4->Lote, nuevaInfo_Car->Lote);
			strcpy_s(aux4->CentroVac, nuevaInfo_Car->CentroVac);
			strcpy_s(aux4->CURPPerson, nuevaInfo_Car->CURPPerson);
			strcpy_s(aux4->ClaveVacCarnet, nuevaInfo_Car->ClaveVacCarnet);
			break;
		}
		aux4 = aux4->sig;
	}
	MessageBox(NULL, "La información de la persona ha sido modificada con éxito.", "Modificar Persona", MB_OK | MB_ICONINFORMATION);
}

//Eliminar 
void EliminarVac(char nomVac[50]) {
	aux2 = inicio2;

	if (aux2 == nullptr) {
		MessageBox(0, "La lista está vacía. Agregue Vacunas.", "AVISO", MB_OK);
	}
	else {
		while (aux2 != nullptr && strcmp(aux2->ClaveVac, nomVac) != 0) {
			aux2 = aux2->sig;
		}
		if (aux2 == nullptr) {
			MessageBox(0, "Vacuna no encontrado", "AVISO", MB_OK);
		}
		else if (aux2 == inicio2) {
			if (aux2->sig == nullptr) {
				inicio2 = nullptr;
				delete aux2;
				aux2 = inicio2;
			}
			else {
				inicio2 = inicio2->sig;
				aux2->sig->ant = nullptr;
				delete aux2;
				aux2 = inicio2;
			}
			MessageBox(0, "Vacuna eliminado", "AVISO", MB_OK);
		}
		else {
			if (aux2->sig == nullptr) {
				aux2->ant->sig = nullptr;
				delete aux2;
				aux2 = inicio2;
			}
			else {
				aux2->sig->ant = aux2->ant;
				aux2->ant->sig = aux2->sig;
				delete aux2;
				aux2 = inicio2;
			}
		}
		MessageBox(0, "Vacuna eliminado", "AVISO", MB_OK);
	}
}
void EliminarPac(char nomPac[50]) {
	aux3 = inicio3;

	if (aux3 == nullptr) {
		MessageBox(0, "La lista está vacía. Agregue Vacunas.", "AVISO", MB_OK);
	}
	else {
		while (aux3 != nullptr && strcmp(aux3->Nombres, nomPac) != 0) {
			aux3 = aux3->sig;
		}
		if (aux3 == nullptr) {
			MessageBox(0, "Vacuna no encontrado", "AVISO", MB_OK);
		}
		else if (aux3 == inicio3) {
			if (aux3->sig == nullptr) {
				inicio3 = nullptr;
				delete aux3;
				aux3 = inicio3;
			}
			else {
				inicio3 = inicio3->sig;
				aux3->sig->ant = nullptr;
				delete aux3;
				aux3 = inicio3;
			}
			MessageBox(0, "Vacuna eliminado", "AVISO", MB_OK);
		}
		else {
			if (aux3->sig == nullptr) {
				aux3->ant->sig = nullptr;
				delete aux3;
				aux3 = inicio3;
			}
			else {
				aux3->sig->ant = aux3->ant;
				aux3->ant->sig = aux3->sig;
				delete aux3;
				aux3 = inicio3;
			}
		}
		MessageBox(0, "Vacuna eliminado", "AVISO", MB_OK);
	}
}
void EliminarCar(char nomCar[50]) {
	aux4 = inicio4;

	if (aux4 == nullptr) {
		MessageBox(0, "La lista está vacía. Agregue Carnet.", "AVISO", MB_OK);
	}
	else {
		while (aux4 != nullptr && (strcmp(aux4->CURPPerson, nomCar) != 0 && strcmp(aux4->ClaveVacCarnet, nomCar) != 0)) {
			aux4 = aux4->sig;
		}
		if (aux4 == nullptr) {
			MessageBox(0, "Carnet no encontrado", "AVISO", MB_OK);
		}
		else if (aux4 == inicio4) {
			if (aux4->sig == nullptr) {
				inicio4 = nullptr;
				fin4 = nullptr;
				delete aux4;
				aux4 = inicio4;
			}
			else {
				inicio4 = inicio4->sig;
				inicio4->ant = nullptr;
				delete aux4;
				aux4 = inicio4;
			}
			MessageBox(0, "Carnet eliminado", "AVISO", MB_OK);
		}
		else if (aux4 == fin4) {
			fin4 = fin4->ant;
			fin4->sig = nullptr;
			delete aux4;
			aux4 = inicio4;
			MessageBox(0, "Carnet eliminado", "AVISO", MB_OK);
		}
		else {
			aux4->ant->sig = aux4->sig;
			aux4->sig->ant = aux4->ant;
			delete aux4;
			aux4 = inicio4;
			MessageBox(0, "Carnet eliminado", "AVISO", MB_OK);
		}
	}
}

//Escribir info en txt
void EscribirUsuario() {
		aux1 = inicio1;
		ofstream escribirUsu;
		escribirUsu.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Primer avance\\usuarios.txt", ios::out | ios::trunc);

		if (escribirUsu.is_open()) {

			escribirUsu << "\t\tUsuarios\n" << endl;
			while (aux1 != nullptr)
			{
				escribirUsu << "Nombre: " << aux1->nomUsu << endl;
				escribirUsu << "Apellido Parterno: " << aux1->ApellidoP << endl;
				escribirUsu << "Apellido Materno: " << aux1->ApellidoM << endl;
				escribirUsu << "Clave: " << aux1->clave << endl;
				escribirUsu << "Contraseña: " << aux1->contra << endl;
				escribirUsu << "-------------------------" << endl;

				aux1 = aux1->sig;
			}
		}
		escribirUsu.close();
	}
void EscribirVacunas() {
	aux2 = inicio2;
	ofstream escribirVacTxt;
	escribirVacTxt.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Primer avance\\Vacunas.txt", ios::out | ios::trunc);

	if (escribirVacTxt.is_open()) {

		escribirVacTxt << "\t\tVacunas\n" << endl;

		while (aux2 != nullptr)
		{
			escribirVacTxt << "Tipo: " << aux2->tipo << endl;
			escribirVacTxt << "Marca: " << aux2->marca << endl;
			escribirVacTxt << "Numero de dosis: " << aux2->NDosis << endl;
			escribirVacTxt << "Clavede vacuna: " << aux2->ClaveVac << endl;
			escribirVacTxt << "Descripcion: " << aux2->Descrip << endl;
			escribirVacTxt << "Precio: " << aux2->precio << endl;
			escribirVacTxt << "-------------------------" << endl;

			aux2 = aux2->sig;
		}
	}
	escribirVacTxt.close();
}
void EscribirPersonas() {
	aux3 = inicio3;
	ofstream escribirPacTxt;
	escribirPacTxt.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Primer avance\\Personas.txt", ios::out | ios::trunc);

	if (escribirPacTxt.is_open()) {

		escribirPacTxt << "\t\tPacientes\n"<< endl;

		while (aux3 != nullptr)
		{
			escribirPacTxt << "Apellido Paterno: " << aux3->ApellidoPP << endl;
			escribirPacTxt << "Apellina Materno: " << aux3->ApellidoMP << endl;
			escribirPacTxt << "Nombres: " << aux3->Nombres << endl;
			escribirPacTxt << "CURP: " << aux3->CURP << endl;
			escribirPacTxt << "RFC: " << aux3->RFC << endl;
			escribirPacTxt << "Calle: " << aux3->Calle << endl;
			escribirPacTxt << "Colonia: " << aux3->Colonia << endl;
			escribirPacTxt << "Ciudad: " << aux3->Ciudad << endl;
			escribirPacTxt << "Estado: " << aux3->Estado << endl;
			escribirPacTxt << "Telefono: " << aux3->telefono << endl;
			escribirPacTxt << "Grupo ocupacional: " << aux3->GrupoOcup << endl;

			if (aux3->EstadoCiv == 1) {
				escribirPacTxt << "Estado civil: Casado(a)" << endl;
			}
			else if (aux3->EstadoCiv == 2) {
				escribirPacTxt << "Estado civil: Conviviente" << endl;
			}
			else if (aux3->EstadoCiv == 3) {
				escribirPacTxt << "Estado civil: Anulado(a)" << endl;
			}
			else if (aux3->EstadoCiv == 4) {
				escribirPacTxt << "Estado civil: Separado de unión legal" << endl;
			}
			else if (aux3->EstadoCiv == 5) {
				escribirPacTxt << "Estado civil: Separado(a) de unión de hecho" << endl;
			}
			else if (aux3->EstadoCiv == 6) {
				escribirPacTxt << "Estado civil: Viudo(a)" << endl;
			}
			else if (aux3->EstadoCiv == 7) {
				escribirPacTxt << "Estado civil: Soltero(a)" << endl;
			}

			if (aux3->PerfilRies == 1) {
				escribirPacTxt << "Perfil de riesgo: Alto" << endl;
			}
			else if (aux3->PerfilRies == 2) {
				escribirPacTxt << "Perfil de riesgo: Medio" << endl;
			}
			else if (aux3->PerfilRies == 3) {
				escribirPacTxt << "Perfil de riesgo: Bajo" << endl;
			}

			if (aux3->Sexo == 1) {
				escribirPacTxt << "Sexo: Hombre" << endl;
			}
			else if (aux3->Sexo == 2) {
				escribirPacTxt << "Sexo: Mujer" << endl;
			}


			escribirPacTxt << "-------------------------" << endl;

			aux3 = aux3->sig;
		}
	}
	escribirPacTxt.close();
}

//Escribir la info en archivo bin para leerla despues 
void EscribirUsuBin() {
	aux1 = inicio1;
	ofstream escribirUsu;
	escribirUsu.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\usuarios.bin", ios::out | ios::binary | ios::trunc);

	if (escribirUsu.is_open()) {
		while (aux1 != nullptr)
		{
			escribirUsu.write((char*)aux1, sizeof(usuario));
			aux1 = aux1->sig;
		}


	}
	escribirUsu.close();
}
void EscribirVacBin() {
	aux2 = inicio2;
	ofstream escribirVac;
	escribirVac.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\vacunas.bin", ios::out | ios::binary | ios::trunc);

	if (escribirVac.is_open()) {
		while (aux2 != nullptr)
		{
			escribirVac.write((char*)aux2, sizeof(vacunas));
			aux2 = aux2->sig;
		}


	}
	escribirVac.close();
}
void EscribirPacBin(){
	aux3 = inicio3;
	ofstream escribirPac;
	escribirPac.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\personas.bin", ios::out | ios::binary | ios::trunc);

	if (escribirPac.is_open()) {
		while (aux3 != nullptr)
		{
			escribirPac.write((char*)aux3, sizeof(personas));
			aux3 = aux3->sig;
		}


	}
	escribirPac.close();
}
void EscribirCarBin() {
	aux4 = inicio4;
	ofstream escribirCar;
	escribirCar.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\Carnet.bin", ios::out | ios::binary | ios::trunc);

	if (escribirCar.is_open()) {
		while (aux4 != nullptr)
		{
			escribirCar.write((char*)aux4, sizeof(carnet));
			aux4 = aux4->sig;
		}


	}
	escribirCar.close();
}

//Leer la info del archivo bin
void LeerUsubina() {

	ifstream leerUsu;
	leerUsu.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\usuarios.bin", ios::in | ios::binary);


	aux1 = inicio1;
	if (leerUsu.is_open()) {

		usuario* usuLeido = new usuario;

		while (!leerUsu.read((char*)usuLeido, sizeof(usuario)).eof()) {

			while (aux1 != nullptr && aux1->sig != nullptr)
			{
				aux1 = aux1->sig;
			}
			if (aux1 == nullptr) {
				inicio1 = usuLeido;
				inicio1->sig = nullptr;
				inicio1->ant = nullptr;
				aux1 = inicio1;
			}
			else {
				aux1->sig = usuLeido;
				aux1->sig->ant = aux1;
				aux1 = aux1->sig;
				aux1->sig = nullptr;
			}

			usuLeido = new usuario;
		}
		leerUsu.close();
	}
}
void LeerVacbina() {

	ifstream leerVac;
	leerVac.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\vacunas.bin", ios::in | ios::binary);


	aux2 = inicio2;
	if (leerVac.is_open()) {

		vacunas* Vacleido = new vacunas;

		while (!leerVac.read((char*)Vacleido, sizeof(vacunas)).eof()) {

			while (aux2 != nullptr && aux2->sig != nullptr)
			{
				aux2 = aux2->sig;
			}
			if (aux2 == nullptr) {
				inicio2 = Vacleido;
				inicio2->sig = nullptr;
				inicio2->ant = nullptr;
				aux2 = inicio2;
			}
			else {
				aux2->sig = Vacleido;
				aux2->sig->ant = aux2;
				aux2 = aux2->sig;
				aux2->sig = nullptr;
			}

			Vacleido = new vacunas;
		}
		leerVac.close();
	}
}
void LeerPacbina() {

	ifstream leerPac;
	leerPac.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\personas.bin", ios::in | ios::binary);


	aux3 = inicio3;
	if (leerPac.is_open()) {

		personas* Pacleido = new personas;

		while (!leerPac.read((char*)Pacleido, sizeof(personas)).eof()) {

			while (aux3 != nullptr && aux3->sig != nullptr)
			{
				aux3 = aux3->sig;
			}
			if (aux3 == nullptr) {
				inicio3 = Pacleido;
				inicio3->sig = nullptr;
				inicio3->ant = nullptr;
				aux3 = inicio3;
			}
			else {
				aux3->sig = Pacleido;
				aux3->sig->ant = aux3;
				aux3 = aux3->sig;
				aux3->sig = nullptr;
			}

			Pacleido = new personas;
		}
		leerPac.close();
	}
}
void LeerCarbina() {

	ifstream leerCar;
	leerCar.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\Carnet.bin", ios::in | ios::binary);


	aux4 = inicio4;
	if (leerCar.is_open()) {

		carnet* Carleido = new carnet;

		while (!leerCar.read((char*)Carleido, sizeof(carnet)).eof()) {

			while (aux4 != nullptr && aux4->sig != nullptr)
			{
				aux4 = aux4->sig;
			}
			if (aux4 == nullptr) {
				inicio4 = Carleido;
				inicio4->sig = nullptr;
				inicio4->ant = nullptr;
				aux4 = inicio4;
			}
			else {
				aux4->sig = Carleido;
				aux4->sig->ant = aux4;
				aux4 = aux4->sig;
				aux4->sig = nullptr;
			}

			Carleido = new carnet;
		}
		leerCar.close();
	}
}

////Busqueda binaria
//carnet* buscarPorID(carnet* inicio4, carnet* fin4, const char* idBuscado) {
//	carnet* ini = inicio4;
//	carnet* fina = fin4;
//
//	while (ini && fina && ini != fina) {
//		carnet* mitad = ini;
//		int tam = 0;
//		while (mitad != fina) {
//			mitad = mitad->sig;
//			tam++;
//		}
//
//		tam /= 2;
//		mitad = ini;
//		for (int i = 0; i < tam; i++) {
//			mitad = mitad->sig;
//		}
//
//		if (strcmp(idBuscado, mitad->IDVac) == 0) {
//			return mitad;
//		}
//		else if (strcmp(idBuscado, mitad->IDVac) < 0) {
//			fina = mitad->ant;
//		}
//		else {
//			ini = mitad->sig;
//		}
//	}
//
//	if (ini && strcmp(idBuscado, ini->IDVac) == 0) {
//		return ini;
//	}
//	else if (fina && strcmp(idBuscado, fina->IDVac) == 0) {
//		return fina;
//	}
//
//	return nullptr;
//}
carnet* buscarPorID(carnet* inicio4, carnet* fin4, const char* idBuscado) {
	carnet* ini = inicio4;
	carnet* fina = fin4;

	while (ini && fina && ini != fina) {
		carnet* mitad = ini;
		int tam = 0;
		while (mitad != fina) {
			mitad = mitad->sig;
			tam++;
		}

		tam /= 2;
		mitad = ini;
		for (int i = 0; i < tam; i++) {
			mitad = mitad->sig;
		}

		if (strcmp(idBuscado, mitad->IDVac) == 0) {
			return mitad;
		}
		else if (strcmp(idBuscado, mitad->IDVac) < 0) {
			fina = mitad->ant;
		}
		else {
			ini = mitad->sig;
		}
	}

	// Agregar condición para verificar si todavía quedan elementos por evaluar
	if (ini && fina && ini == fina && strcmp(idBuscado, ini->IDVac) == 0) {
		return ini;
	}
	else if (fina && strcmp(idBuscado, fina->IDVac) == 0) {
		return fina;
	}
	else if (ini && strcmp(idBuscado, ini->IDVac) == 0) {
		return ini;
	}

	return nullptr;
}

//Ordenar por heapsort
void AgregarElementosListBox(HWND hwndDlg, carnet* inicio, bool ordenar) {
	SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_RESETCONTENT, 0, 0);
	carnet* aux4 = inicio;

	if (ordenar) {
		// Contamos el número de elementos de la lista
		int numElementos = 0;
		while (aux4 != nullptr) {
			numElementos++;
			aux4 = aux4->sig;
		}

		// Creamos un arreglo temporal para almacenar los elementos de la lista
		carnet** arr = new carnet * [numElementos];

		// Copiamos los elementos de la lista al arreglo temporal
		aux4 = inicio;
		for (int i = 0; i < numElementos; i++) {
			arr[i] = aux4;
			aux4 = aux4->sig;
		}

		// Ordenamos el arreglo temporal por el ID utilizando HeapSort
		for (int i = numElementos / 2 - 1; i >= 0; i--) {
			heapify(arr, numElementos, i);
		}

		for (int i = numElementos - 1; i >= 0; i--) {
			std::swap(arr[0], arr[i]);
			heapify(arr, i, 0);
		}

		// Agregamos los elementos ordenados al ListBox
		for (int i = 0; i < numElementos; i++) {
			char textoList[100];
			sprintf_s(textoList, sizeof(textoList), "%s - %s", arr[i]->IDVac, arr[i]->CURPPerson);
			SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_ADDSTRING, 0, (LPARAM)textoList);
		}

		ofstream escribirheap;
		escribirheap.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\Reportes.csv", ios::out);

		if (escribirheap.is_open()) {
			escribirheap << "\tLISTADO DE CARNETS\n" << endl;
			for (int i = 0; i < numElementos; i++) {
				SYSTEMTIME fechaVac = arr[i]->FechaVac;
				tm fechaVac_tm = {};
				fechaVac_tm.tm_year = fechaVac.wYear - 1900;
				fechaVac_tm.tm_mon = fechaVac.wMonth - 1;
				fechaVac_tm.tm_mday = fechaVac.wDay;
				fechaVac_tm.tm_hour = fechaVac.wHour;
				fechaVac_tm.tm_min = fechaVac.wMinute;
				fechaVac_tm.tm_sec = fechaVac.wSecond;
				fechaVac_tm.tm_isdst = -1; // Indicar que no se conoce si hay horario de verano

				char fechaVacStr[80];
				strftime(fechaVacStr, 80, "%Y-%m-%d", &fechaVac_tm);
				escribirheap << "-----------------------------------------------\n" << endl;
				escribirheap << "CURP:\t" << arr[i]->CURPPerson << endl;
				escribirheap << "VACUNA:\t" << arr[i]->ClaveVacCarnet << endl;
				escribirheap << "ID DE CARNE:\t" << arr[i]->IDVac << endl;
				escribirheap << "CENTRO DE VACUNACION:\t" << arr[i]->CentroVac << endl;
				escribirheap << "FECHAS DE VACUNACION:\t" << fechaVacStr << endl;
				escribirheap << "LOTE:\t" << arr[i]->Lote << endl;
			}
			escribirheap.close();
			}
		else {

			system("PAUSE");
		}

		// Liberamos la memoria del arreglo temporal
		delete[] arr;
	}
	else {
		// Agregamos los elementos tal y como están en la lista al ListBox
		while (aux4 != nullptr) {
			char textoList[100];
			sprintf_s(textoList, sizeof(textoList), "%s - %s", aux4->IDVac, aux4->CURPPerson);
			SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_ADDSTRING, 0, (LPARAM)textoList);
			aux4 = aux4->sig;
		}
	}
}
void heapify(carnet** arr, int n, int i) {
	int largest = i;
	int left = 2 * i + 1;
	int right = 2 * i + 2;

	if (left < n && atoi(arr[left]->IDVac) > atoi(arr[largest]->IDVac))
		largest = left;

	if (right < n && atoi(arr[right]->IDVac) > atoi(arr[largest]->IDVac))
		largest = right;

	if (largest != i) {
		std::swap(arr[i], arr[largest]);
		heapify(arr, n, largest);
	}
}

//Ordenar por quicksort
void AgregarElementosListBoxCURP(HWND hwndDlg, carnet* inicio, bool ordenar) {
	SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_RESETCONTENT, 0, 0);
	carnet* aux4 = inicio;

	if (ordenar) {
		// Contamos el número de elementos de la lista
		int numElementos = 0;
		while (aux4 != nullptr) {
			numElementos++;
			aux4 = aux4->sig;
		}

		// Creamos un arreglo temporal para almacenar los elementos de la lista
		carnet** arr = new carnet * [numElementos];

		// Copiamos los elementos de la lista al arreglo temporal
		aux4 = inicio;
		for (int i = 0; i < numElementos; i++) {
			arr[i] = aux4;
			aux4 = aux4->sig;
		}

		// Ordenamos el arreglo temporal por la CURP utilizando Quicksort
		quicksort(arr, 0, numElementos - 1);

		// Agregamos los elementos ordenados al ListBox
		for (int i = 0; i < numElementos; i++) {
			char textoList[100];
			sprintf_s(textoList, sizeof(textoList), "%s - %s", arr[i]->CURPPerson, arr[i]->IDVac);
			SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_ADDSTRING, 0, (LPARAM)textoList);
		}


		// Liberamos la memoria del arreglo temporal
		delete[] arr;
	}
	else {
		// Agregamos los elementos tal y como están en la lista al ListBox
		while (aux4 != nullptr) {
			char textoList[100];
			sprintf_s(textoList, sizeof(textoList), "%s - %s", aux4->CURPPerson, aux4->IDVac);
			SendMessage(GetDlgItem(hwndDlg, IDC_LISTMostrarCarnet), LB_ADDSTRING, 0, (LPARAM)textoList);
			aux4 = aux4->sig;
		}
	}
}
// Función para intercambiar dos elementos de un arreglo
void swap(carnet** arr, int i, int j) {
	carnet* temp = arr[i];
	arr[i] = arr[j];
	arr[j] = temp;
}
// Función para particionar el arreglo y seleccionar el pivote
int partition(carnet** arr, int low, int high) {
	char* pivot = arr[high]->CURPPerson;
	int i = low - 1;

	for (int j = low; j < high; j++) {
		if (strcmp(arr[j]->CURPPerson, pivot) < 0 || (strcmp(arr[j]->CURPPerson, pivot) == 0 && strcmp(arr[j]->IDVac, arr[high]->IDVac) < 0)) {
			i++;
			swap(arr, i, j);
		}
	}

	swap(arr, i + 1, high);

	return i + 1;
}
// Función recursiva para realizar el Quicksort
void quicksort(carnet** arr, int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);

		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}

void CrearArchivoCSVQuick(carnet** arr, int numElementos) {
	// Creamos un objeto ofstream para escribir en el archivo CSV

	ofstream escribirquick;
	escribirquick.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\Quick.csv", ios::out);

	if (escribirquick.is_open()) {
		escribirquick << "Listado de carnets\n" << endl;
		for (int i = 0; i < numElementos; i++) {
			SYSTEMTIME fechaVac = arr[i]->FechaVac;
			tm fechaVac_tm = {};
			fechaVac_tm.tm_year = fechaVac.wYear - 1900;
			fechaVac_tm.tm_mon = fechaVac.wMonth - 1;
			fechaVac_tm.tm_mday = fechaVac.wDay;
			fechaVac_tm.tm_hour = fechaVac.wHour;
			fechaVac_tm.tm_min = fechaVac.wMinute;
			fechaVac_tm.tm_sec = fechaVac.wSecond;
			fechaVac_tm.tm_isdst = -1; // Indicar que no se conoce si hay horario de verano

			char fechaVacStr[80];
			strftime(fechaVacStr, 80, "%Y-%m-%d", &fechaVac_tm);
			escribirquick << "-----------------------------------------------\n" << endl;
			escribirquick << "CURP: " << arr[i]->CURPPerson << endl;
			escribirquick << "VACUNA: " << arr[i]->ClaveVacCarnet << endl;
			escribirquick << "ID DE CARNE : " << arr[i]->IDVac << endl;
			escribirquick << "CENTRO DE VACUNACION : " << arr[i]->CentroVac << endl;
			escribirquick << "FECHAS DE VACUNACION: " << fechaVacStr << endl;
			escribirquick << "LOTE: " << arr[i]->Lote << endl;
		}
		escribirquick.close();
	}
	else {

		system("PAUSE");
	}
}

void ReporteQuickCSV(HWND hwndDlg, carnet* inicio, bool ordenar) {
	carnet* aux4 = inicio;

	if (ordenar) {
		// Contamos el número de elementos de la lista
		int numElementos = 0;
		while (aux4 != nullptr) {
			numElementos++;
			aux4 = aux4->sig;
		}

		// Creamos un arreglo temporal para almacenar los elementos de la lista
		carnet** arr = new carnet * [numElementos];

		// Copiamos los elementos de la lista al arreglo temporal
		aux4 = inicio;
		for (int i = 0; i < numElementos; i++) {
			arr[i] = aux4;
			aux4 = aux4->sig;
		}

		// Ordenamos el arreglo temporal por la CURP utilizando Quicksort
		quicksort(arr, 0, numElementos - 1);

		ofstream escribirquick;
		escribirquick.open("D:\\jjdel\\Escritorio\\4to semestre\\Estructura de datos\\Segundo avance\\Primer avance 1\\Base de datos en binario\\Quick.csv", ios::out);

		if (escribirquick.is_open()) {
			escribirquick << "Listado de carnets\n" << endl;
			for (int i = 0; i < numElementos; i++) {
				SYSTEMTIME fechaVac = arr[i]->FechaVac;
				tm fechaVac_tm = {};
				fechaVac_tm.tm_year = fechaVac.wYear - 1900;
				fechaVac_tm.tm_mon = fechaVac.wMonth - 1;
				fechaVac_tm.tm_mday = fechaVac.wDay;
				fechaVac_tm.tm_hour = fechaVac.wHour;
				fechaVac_tm.tm_min = fechaVac.wMinute;
				fechaVac_tm.tm_sec = fechaVac.wSecond;
				fechaVac_tm.tm_isdst = -1; // Indicar que no se conoce si hay horario de verano

				char fechaVacStr[80];
				strftime(fechaVacStr, 80, "%Y-%m-%d", &fechaVac_tm);
				escribirquick << "-----------------------------------------------\n" << endl;
				escribirquick << "CURP: " << arr[i]->CURPPerson << endl;
				escribirquick << "VACUNA: " << arr[i]->ClaveVacCarnet << endl;
				escribirquick << "ID DE CARNE : " << arr[i]->IDVac << endl;
				escribirquick << "CENTRO DE VACUNACION : " << arr[i]->CentroVac << endl;
				escribirquick << "FECHAS DE VACUNACION: " << fechaVacStr << endl;
				escribirquick << "LOTE: " << arr[i]->Lote << endl;

				escribirquick.flush();
			}
			escribirquick.close();
		}
		else {

			system("PAUSE");
		}

		delete[] arr;
	}
}