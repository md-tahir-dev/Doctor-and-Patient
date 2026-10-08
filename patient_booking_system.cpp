// Patient Booking and Doctor Schedule System
// Programming Fundamentals Project
//
// Doctors have a visiting schedule, patients can book or cancel
// appointments with them. Everything is saved in text files so
// the data is still there next time the program runs.
//
// Files used (one record per line, fields split by '|'):
// doctors.txt, patients.txt, appointments.txt
//
// Uses plain fixed-size arrays instead of vectors, so the max
// number of doctors/patients/appointments is set ahead of time.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// ---------- max sizes for our arrays ----------

const int MAX_DOCTORS = 100;
const int MAX_PATIENTS = 200;
const int MAX_APPOINTMENTS = 500;

// ---------- data structures ----------

struct Doctor {
    int id;//1, 2, 3 or any integer
    string name;
    string specialization;
    string day;   // day they see patients, e.g. Monday
    string time;  // e.g. 10:00 AM
};

struct Patient {
    int id;
    string name;
    int age;
    string contact;
};

// this struct is what links a doctor and a patient together
struct Appointment {
    int appointmentId;
    int doctorId;
    int patientId;
    string date;
    string time;
    bool active; // false means cancelled
};

Doctor doctors[MAX_DOCTORS];
int doctorCount = 0;

Patient patients[MAX_PATIENTS];
int patientCount = 0;

Appointment appointments[MAX_APPOINTMENTS];
int appointmentCount = 0;

int nextAppointmentId = 1;

string DOCTOR_FILE = "doctors.txt";
string PATIENT_FILE = "patients.txt";
string APPOINTMENT_FILE = "appointments.txt";

// ---------- simple input helpers ----------

// keeps asking until the user types a positive whole number
int inputPositiveInt(string prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail() || value <= 0) {
            cin.clear();
            cin.ignore(1000, '\n'); // throw away the bad input
            cout << "Invalid input. Please enter a positive number.\n";
        } else {
            cin.ignore(1000, '\n'); // clear the leftover newline
            return value;
        }
    }
}

// keeps asking until the user types something (not blank)
string inputNonEmptyLine(string prompt) {
    string value;
    while (true) {
        cout << prompt;
        getline(cin, value);
        if (value == "") {
            cout << "This can't be empty, try again.\n";
        } else {
            return value;
        }
    }
}

// ---------- lookup helpers ----------

// returns the position of a doctor in the array, or -1 if not there
int findDoctorIndex(int id) {
    for (int i = 0; i < doctorCount; i++) {
        if (doctors[i].id == id) {
            return i;
        }
    }
    return -1;
}

int findPatientIndex(int id) {
    for (int i = 0; i < patientCount; i++) {
        if (patients[i].id == id) {
            return i;
        }
    }
    return -1;
}

string getDoctorName(int id) {
    int idx = findDoctorIndex(id);
    if (idx == -1) {
        return "(Unknown Doctor)";
    }
    return doctors[idx].name;
}

string getPatientName(int id) {
    int idx = findPatientIndex(id);
    if (idx == -1) {
        return "(Unknown Patient)";
    }
    return patients[idx].name;
}

// true if this doctor already has an active appointment on this date
bool isSlotTaken(int doctorId, string date) {
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].active == true &&
            appointments[i].doctorId == doctorId &&
            appointments[i].date == date) {
            return true;
        }
    }
    return false;
}

// ---------- saving and loading files ----------

void saveDoctors() {
    ofstream fout(DOCTOR_FILE.c_str());
    for (int i = 0; i < doctorCount; i++) {
        fout << doctors[i].id << "|" << doctors[i].name << "|"
             << doctors[i].specialization << "|" << doctors[i].day << "|"
             << doctors[i].time << "\n";
    }
}

void loadDoctors() {
    doctorCount = 0;
    ifstream fin(DOCTOR_FILE.c_str());
    string line;
    while (getline(fin, line) && doctorCount < MAX_DOCTORS) {
        if (line == "") continue;

        stringstream ss(line);
        string idStr, name, spec, day, time;
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, spec, '|');
        getline(ss, day, '|');
        getline(ss, time, '|');

        doctors[doctorCount].id = stoi(idStr);
        doctors[doctorCount].name = name;
        doctors[doctorCount].specialization = spec;
        doctors[doctorCount].day = day;
        doctors[doctorCount].time = time;
        doctorCount = doctorCount + 1;
    }
}

void savePatients() {
    ofstream fout(PATIENT_FILE.c_str());
    for (int i = 0; i < patientCount; i++) {
        fout << patients[i].id << "|" << patients[i].name << "|"
             << patients[i].age << "|" << patients[i].contact << "\n";
    }
}

void loadPatients() {
    patientCount = 0;
    ifstream fin(PATIENT_FILE.c_str());
    string line;
    while (getline(fin, line) && patientCount < MAX_PATIENTS) {
        if (line == "") continue;

        stringstream ss(line);
        string idStr, name, ageStr, contact;
        getline(ss, idStr, '|');
        getline(ss, name, '|');
        getline(ss, ageStr, '|');
        getline(ss, contact, '|');

        patients[patientCount].id = stoi(idStr);
        patients[patientCount].name = name;
        patients[patientCount].age = stoi(ageStr);
        patients[patientCount].contact = contact;
        patientCount = patientCount + 1;
    }
}

void saveAppointments() {
    ofstream fout(APPOINTMENT_FILE.c_str());
    for (int i = 0; i < appointmentCount; i++) {
        int activeFlag = 0;
        if (appointments[i].active == true) {
            activeFlag = 1;
        }
        fout << appointments[i].appointmentId << "|" << appointments[i].doctorId
             << "|" << appointments[i].patientId << "|" << appointments[i].date
             << "|" << appointments[i].time << "|" << activeFlag << "\n";
    }
}

void loadAppointments() {
    appointmentCount = 0;
    ifstream fin(APPOINTMENT_FILE.c_str());
    string line;
    while (getline(fin, line) && appointmentCount < MAX_APPOINTMENTS) {
        if (line == "") continue;

        stringstream ss(line);
        string apptIdStr, docIdStr, patIdStr, date, time, activeStr;
        getline(ss, apptIdStr, '|');
        getline(ss, docIdStr, '|');
        getline(ss, patIdStr, '|');
        getline(ss, date, '|');
        getline(ss, time, '|');
        getline(ss, activeStr, '|');

        appointments[appointmentCount].appointmentId = stoi(apptIdStr);
        appointments[appointmentCount].doctorId = stoi(docIdStr);
        appointments[appointmentCount].patientId = stoi(patIdStr);
        appointments[appointmentCount].date = date;
        appointments[appointmentCount].time = time;
        if (activeStr == "1") {
            appointments[appointmentCount].active = true;
        } else {
            appointments[appointmentCount].active = false;
        }

        // keep the ID counter ahead of anything we just loaded
        if (appointments[appointmentCount].appointmentId >= nextAppointmentId) {
            nextAppointmentId = appointments[appointmentCount].appointmentId + 1;
        }

        appointmentCount = appointmentCount + 1;
    }
}

// ---------- doctor section ----------

void addDoctor() {
    cout << "\n--- Add Doctor ---\n";

    if (doctorCount >= MAX_DOCTORS) {
        cout << "Doctor list is full, can't add more.\n";
        return;
    }

    int id = inputPositiveInt("Enter Doctor ID: ");

    if (findDoctorIndex(id) != -1) {
        cout << "A doctor with this ID already exists.\n";
        return;
    }

    doctors[doctorCount].id = id;
    doctors[doctorCount].name = inputNonEmptyLine("Enter Doctor Name: ");
    doctors[doctorCount].specialization = inputNonEmptyLine("Enter Specialization: ");
    doctors[doctorCount].day = inputNonEmptyLine("Enter Visiting Day (e.g. Monday): ");
    doctors[doctorCount].time = inputNonEmptyLine("Enter Visiting Time (e.g. 10:00 AM): ");

    doctorCount = doctorCount + 1;
    saveDoctors();
    cout << "Doctor added.\n";
}

void viewAllDoctors() {
    cout << "\n--- All Doctors ---\n";
    if (doctorCount == 0) {
        cout << "No doctors found.\n";
        return;
    }
    for (int i = 0; i < doctorCount; i++) {
        cout << "ID: " << doctors[i].id << " | Name: " << doctors[i].name
             << " | Specialization: " << doctors[i].specialization
             << " | Day: " << doctors[i].day
             << " | Time: " << doctors[i].time << "\n";
    }
}

// shows one doctor's info plus everyone who booked with them
void viewDoctorSchedule() {
    cout << "\n--- Doctor Schedule ---\n";
    int id = inputPositiveInt("Enter Doctor ID: ");
    int idx = findDoctorIndex(id);
    if (idx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Doctor: " << doctors[idx].name << " (" << doctors[idx].specialization << ")\n";
    cout << "Visits: " << doctors[idx].day << " / " << doctors[idx].time << "\n";
    cout << "Appointments:\n";

    bool found = false;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].doctorId == id) {
            found = true;
            string status = "Cancelled";
            if (appointments[i].active == true) {
                status = "Active";
            }
            cout << "  #" << appointments[i].appointmentId
                 << " | Patient: " << getPatientName(appointments[i].patientId)
                 << " | Date: " << appointments[i].date
                 << " | " << status << "\n";
        }
    }
    if (found == false) {
        cout << "  Nothing booked with this doctor yet.\n";
    }
}

void updateDoctor() {
    cout << "\n--- Update Doctor ---\n";
    int id = inputPositiveInt("Enter Doctor ID to update: ");
    int idx = findDoctorIndex(id);
    if (idx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    doctors[idx].name = inputNonEmptyLine("Enter new Name: ");
    doctors[idx].specialization = inputNonEmptyLine("Enter new Specialization: ");
    doctors[idx].day = inputNonEmptyLine("Enter new Visiting Day: ");
    doctors[idx].time = inputNonEmptyLine("Enter new Visiting Time: ");

    saveDoctors();
    cout << "Doctor updated.\n";
}

void deleteDoctor() {
    cout << "\n--- Delete Doctor ---\n";
    int id = inputPositiveInt("Enter Doctor ID to delete: ");
    int idx = findDoctorIndex(id);
    if (idx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    // shift every doctor after this one back by one spot to "remove" it
    for (int i = idx; i < doctorCount - 1; i++) {
        doctors[i] = doctors[i + 1];
    }
    doctorCount = doctorCount - 1;

    // don't delete their appointment history, just mark it cancelled
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].doctorId == id) {
            appointments[i].active = false;
        }
    }

    saveDoctors();
    saveAppointments();
    cout << "Doctor deleted, related appointments cancelled.\n";
}

// ---------- patient section ----------

void registerPatient() {
    cout << "\n--- Register Patient ---\n";

    if (patientCount >= MAX_PATIENTS) {
        cout << "Patient list is full, can't add more.\n";
        return;
    }

    int id = inputPositiveInt("Enter Patient ID: ");

    if (findPatientIndex(id) != -1) {
        cout << "A patient with this ID already exists.\n";
        return;
    }

    patients[patientCount].id = id;
    patients[patientCount].name = inputNonEmptyLine("Enter Patient Name: ");
    patients[patientCount].age = inputPositiveInt("Enter Patient Age: ");
    patients[patientCount].contact = inputNonEmptyLine("Enter Contact Number: ");

    patientCount = patientCount + 1;
    savePatients();
    cout << "Patient registered.\n";
}

void viewAllPatients() {
    cout << "\n--- All Patients ---\n";
    if (patientCount == 0) {
        cout << "No patients found.\n";
        return;
    }
    for (int i = 0; i < patientCount; i++) {
        cout << "ID: " << patients[i].id << " | Name: " << patients[i].name
             << " | Age: " << patients[i].age
             << " | Contact: " << patients[i].contact << "\n";
    }
}

// patient picks a doctor and date, we check the slot is free, then save
void bookAppointment() {
    cout << "\n--- Book Appointment ---\n";

    if (appointmentCount >= MAX_APPOINTMENTS) {
        cout << "Appointment list is full, can't book more.\n";
        return;
    }

    int patientId = inputPositiveInt("Enter your Patient ID: ");
    if (findPatientIndex(patientId) == -1) {
        cout << "Patient not found, please register first.\n";
        return;
    }

    int doctorId = inputPositiveInt("Enter Doctor ID: ");
    int docIdx = findDoctorIndex(doctorId);
    if (docIdx == -1) {
        cout << "Doctor not found.\n";
        return;
    }

    cout << "Doctor " << doctors[docIdx].name << " visits on "
         << doctors[docIdx].day << " at " << doctors[docIdx].time << "\n";

    string date = inputNonEmptyLine("Enter appointment date (e.g. 2026-08-15): ");

    if (isSlotTaken(doctorId, date) == true) {
        cout << "This doctor is already booked on that date, pick another.\n";
        return;
    }

    appointments[appointmentCount].appointmentId = nextAppointmentId;
    nextAppointmentId = nextAppointmentId + 1;
    appointments[appointmentCount].doctorId = doctorId;
    appointments[appointmentCount].patientId = patientId;
    appointments[appointmentCount].date = date;
    appointments[appointmentCount].time = doctors[docIdx].time; // doctor's fixed time
    appointments[appointmentCount].active = true;

    int newApptId = appointments[appointmentCount].appointmentId;
    appointmentCount = appointmentCount + 1;

    saveAppointments();
    cout << "Booked! Your appointment ID is " << newApptId << "\n";
}

void viewOwnAppointmentHistory() {
    cout << "\n--- Appointment History ---\n";
    int patientId = inputPositiveInt("Enter your Patient ID: ");
    if (findPatientIndex(patientId) == -1) {
        cout << "Patient not found.\n";
        return;
    }

    bool found = false;
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].patientId == patientId) {
            found = true;
            string status = "Cancelled";
            if (appointments[i].active == true) {
                status = "Active";
            }
            cout << "#" << appointments[i].appointmentId
                 << " | Doctor: " << getDoctorName(appointments[i].doctorId)
                 << " | Date: " << appointments[i].date
                 << " | " << status << "\n";
        }
    }
    if (found == false) {
        cout << "No appointments found for this patient.\n";
    }
}

void cancelAppointment() {
    cout << "\n--- Cancel Appointment ---\n";
    int patientId = inputPositiveInt("Enter your Patient ID: ");
    if (findPatientIndex(patientId) == -1) {
        cout << "Patient not found.\n";
        return;
    }

    bool found = false;
    cout << "Your active appointments:\n";
    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].patientId == patientId && appointments[i].active == true) {
            found = true;
            cout << "  #" << appointments[i].appointmentId
                 << " | Doctor: " << getDoctorName(appointments[i].doctorId)
                 << " | Date: " << appointments[i].date << "\n";
        }
    }
    if (found == false) {
        cout << "You have no active appointments.\n";
        return;
    }

    int apptId = inputPositiveInt("Enter Appointment ID to cancel: ");

    for (int i = 0; i < appointmentCount; i++) {
        if (appointments[i].appointmentId == apptId &&
            appointments[i].patientId == patientId &&
            appointments[i].active == true) {
            appointments[i].active = false;
            saveAppointments();
            cout << "Cancelled.\n";
            return;
        }
    }
    cout << "Couldn't find that appointment (or it's already cancelled).\n";
}

// ---------- menus ----------

void doctorMenu() {
    int choice;
    do {
        cout << "\n===== DOCTOR SECTION =====\n";
        cout << "1. Add Doctor\n";
        cout << "2. View All Doctors\n";
        cout << "3. View a Doctor's Schedule\n";
        cout << "4. Update Doctor Info\n";
        cout << "5. Delete Doctor\n";
        cout << "6. Back to Main Menu\n";
        choice = inputPositiveInt("Enter your choice: ");

        switch (choice) {
            case 1:
                addDoctor();
                break;
            case 2:
                viewAllDoctors();
                break;
            case 3:
                viewDoctorSchedule();
                break;
            case 4:
                updateDoctor();
                break;
            case 5:
                deleteDoctor();
                break;
            case 6:
                cout << "Going back...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 6);
}

void patientMenu() {
    int choice;
    do {
        cout << "\n===== PATIENT SECTION =====\n";
        cout << "1. Register Patient\n";
        cout << "2. Book Appointment\n";
        cout << "3. View Own Appointment History\n";
        cout << "4. Cancel Appointment\n";
        cout << "5. View All Patients\n";
        cout << "6. Back to Main Menu\n";
        choice = inputPositiveInt("Enter your choice: ");

        switch (choice) {
            case 1:
                registerPatient();
                break;
            case 2:
                bookAppointment();
                break;
            case 3:
                viewOwnAppointmentHistory();
                break;
            case 4:
                cancelAppointment();
                break;
            case 5:
                viewAllPatients();
                break;
            case 6:
                cout << "Going back...\n";
                break;
            default:
                cout << "Invalid choice.\n";
                break;
        }
    } while (choice != 6);
}

int main() {
    loadDoctors();
    loadPatients();
    loadAppointments();

    cout << "=====================================================\n";
    cout << "  PATIENT BOOKING AND DOCTOR SCHEDULE SYSTEM\n";
    cout << "=====================================================\n";

    int choice;
    do {
        cout << "\n===== MAIN MENU =====\n";
        cout << "1. Doctor Section\n";
        cout << "2. Patient Section\n";
        cout << "3. Exit\n";
        choice = inputPositiveInt("Enter your choice: ");

        switch (choice) {
            case 1:
                doctorMenu();
                break;
            case 2:
                patientMenu();
                break;
            case 3:
                cout << "Goodbye! All data has been saved.\n";
                break;
            default:
                cout << "Please enter 1, 2, or 3.\n";
                break;
        }
    } while (choice != 3);

    return 0;
}
