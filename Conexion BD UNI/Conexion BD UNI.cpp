// ConexionBD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    SQLHENV hEnv;
    SQLHDBC hDbc;
    SQLRETURN ret;

    // Allocate an environment handle
    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    ret = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);

    // Allocate a connection handle
    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    // Connect to the database
    ret = SQLConnect(hDbc, (SQLWCHAR*)L"sqlserver", SQL_NTS, (SQLWCHAR*)L"Username", SQL_NTS, (SQLWCHAR*)L"Password", SQL_NTS);

    if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
        cout << "Connected to database successfully." << endl;


        // Example of executing a query
        SQLHSTMT hStmt;
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

        // print table headers
        cout << "+---------+----------------------------+-------------+-------------+-----------------+-----------------+-----------------+" << endl;
        cout << "| No. Emp |      Nombre Completo       |  Fecha Nac  |     RFC     |  Nombre Centro  | Desc del puesto |  Es Directivo?  |" << endl;
        cout << "+---------+----------------------------+-------------+-------------+-----------------+-----------------+-----------------+" << endl;
               
        // Example SELECT query
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Empleado", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLCHAR nombre[50];
            SQLCHAR a_paterno[50];
            SQLCHAR a_materno[50];
            SQLCHAR fecha_nac[50];
            SQLCHAR rfc[50];
            SQLCHAR puesto[100];
            SQLCHAR nom_cent[50];
            SQLCHAR direc[10];

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre, sizeof(nombre), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, a_paterno, sizeof(a_paterno), NULL);
                SQLGetData(hStmt, 4, SQL_C_CHAR, a_materno, sizeof(a_materno), NULL);
                SQLGetData(hStmt, 5, SQL_C_CHAR, fecha_nac, sizeof(fecha_nac), NULL);
                SQLGetData(hStmt, 6, SQL_C_CHAR, rfc, sizeof(rfc), NULL);
                SQLGetData(hStmt, 7, SQL_C_CHAR, puesto, sizeof(puesto), NULL);
                SQLGetData(hStmt, 9, SQL_C_CHAR, nom_cent, sizeof(nom_cent), NULL);
                SQLGetData(hStmt, 10, SQL_C_CHAR, direc, sizeof(direc), NULL);

                // concatenete paternal name and surname
                string last_names = string((char*)a_paterno) + " " + string((char*)a_materno);
                string full_name = string((char*)nombre) + " " + string(last_names);

                // manager change to yes and no
                string dir = string((char*)direc);
                string respuesta;

                if (dir == "1") {
                    respuesta = "SI";
                }
                else
                {
                    respuesta = "NO";
                }


                // Print row data with alignment
                cout << "| " << setw(8) << left << num_empleado
                     << "| " << setw(27) << left << full_name
                     << "| " << setw(12) << left << fecha_nac
                     << "| " << setw(12) << left << rfc
                     << "| " << setw(16) << left << nom_cent
                     << "| " << setw(16) << left << puesto
                     << "| " << setw(15) << left << respuesta << " |" << endl;
            }
        }

        // print final line of the table
        cout << "+---------+----------------------------+-------------+-------------+-----------------+-----------------+-----------------+" << endl;

         
        // Free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        
        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        cout << "+-------------------------------------------------------------------+" << endl;
        cout << "|  Num_Centro " << "|      Nombre_Centro " << "            |   Ciudad_Centro    |" << endl;
        // Example SELECT query
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Centro", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_centro;
            SQLCHAR nombre_centro[100];
            SQLCHAR ciudad_centro[80];
            
            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_centro, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, nombre_centro, sizeof(nombre_centro), NULL);
                SQLGetData(hStmt, 3, SQL_C_CHAR, ciudad_centro, sizeof(ciudad_centro), NULL);
                
                cout << "| " << num_centro << "           " << nombre_centro << "              " << ciudad_centro << endl;

            }
        }

        // Free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

        ret = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
        cout << "+--------------------------------------+" << endl;
        cout << "| Num_Empleado " << "| Pres_Gas" << " | Num_Centro |" << endl;
        // Example SELECT query
        ret = SQLExecDirect(hStmt, (SQLWCHAR*)L"SELECT * FROM Directivo", SQL_NTS);
        if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
            int num_empleado;
            SQLCHAR prestacion_gasolina[10];
            int num_centro;

            while (SQLFetch(hStmt) == SQL_SUCCESS) {
                SQLGetData(hStmt, 1, SQL_C_LONG, &num_empleado, 0, NULL);
                SQLGetData(hStmt, 2, SQL_C_CHAR, prestacion_gasolina, sizeof(prestacion_gasolina), NULL);
                SQLGetData(hStmt, 3, SQL_C_LONG, &num_centro, 0, NULL);

                cout << "| " <<  num_empleado << "           " << prestacion_gasolina << "          " << num_centro << endl;

            }
        }

        // Free statement handle
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    }
    else {
        cout << "Failed to connect to database." << endl;
    }

    // Disconnect and free handles
    SQLDisconnect(hDbc);
    SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    return 0;
}