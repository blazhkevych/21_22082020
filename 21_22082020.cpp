#include <iostream>
#include <time.h>
#include <fstream>
using namespace std;

enum People
{
	error = 0,
	student,
	worker,
	teacher
};
class Human
{
protected:
	char* name;
	int age;
public:
	Human() :name(0), age(0) {};
	Human(const char* name, int age) :
		name(_strdup(name)), age(age) { };
	virtual ~Human() {
		delete[] name;
	}
	Human(const Human& R);
	virtual void Show() = 0;
	virtual void Save() = 0;
	virtual void Load(int& offset) = 0;
};

Human::Human(const Human& R) :Human(R.name, R.age) { }

void Human::Show()
{
	cout << (name ? name : "No name.");
	cout << " - " << age;
}

class Student : public Human
{
private:
	char* univer;
	int* marks;
	int countMarks;
public:
	Student() :Student(0, 0, 0, 0) {};
	Student(const char* name, int age, const char* univer, int countMarks) :
		Human(name, age), countMarks(countMarks), univer(_strdup(univer)) {
		marks = new int[countMarks] {};
		for (int i = 0; i < countMarks; i++)
			marks[i] = 1 + rand() % 12;
	};
	~Student() {
		delete[] univer;
		delete[] marks;
	}
	double AVG() {
		double avg = 0;
		for (int i = 0; i < countMarks; i++)
			avg += marks[i];
		return avg / countMarks;
	};
	void Show() override {
		Human::Show();
		cout << " - " << (univer ? univer : "No univer");
		cout << " - ";
		for (int i = 0; i < countMarks; i++)
			cout << marks[i] << " ";
		cout << " Avg - " << AVG() << endl;
	};
	void Save() override;
	void Load(int& offset) override;
};

class Worker : public Human
{
private:
	char* workPlace;
	double salary;
public:
	Worker() : Worker(0, 0, 0, 0) {};
	Worker(const char* name, int age, const char* workPlace, double salary) :
		Human(name, age), workPlace(_strdup(workPlace)), salary(salary) { };
	~Worker() {
		delete[] workPlace;
	}
	void Show() override {
		Human::Show();
		cout << " - " << (workPlace ? workPlace : "No place of work.");
		cout << " - " << salary << endl;
	};
	void Save() override;
	void Load(int& offset) override;
};

void Save();
void Load();

int main()
{
	srand(time(0));
	//Student s("Vasya Ivanov", 23, "IT STEP", 5);
	//s.Show();
	//s.Save();
	//s.Load();
	/*int offset = 0;
	Student b;
	b.Load(offset);
	b.Show();*/
	/*Worker work("Petro Stepanovich", 42, "Zavod Z", 322.28);
	work.Show();
	work.Save();*/
	//Worker w;
	//w.Load(offset);
	//w.Show();

	Save();
	Load();

}

void Save()
{
	const int N = 4;
	Human** h = new Human * [N];
	for (int i = 0; i < N; i++)
	{
		int r = rand() % 2;
		switch (r)
		{
		case 0:
			h[i] = new Worker("Petro WORKER", 42, "Zavod Z", 322.28);
			break;
		case 1:
			h[i] = new Student("Vasya STUDENT", 23, "IT STEP", 5);
			break;
		}
		h[i]->Save();
	}
	for (int i = 0; i < N; i++)
		delete[] h[i];
	delete[] h;

}

void Load() {
	ifstream fcin("f.bin", ios::binary);
	int offset = 0;
	//Human** h = nullptr;
	do {

		Human* temp = nullptr;
		People who = error;

		fcin.read((char*)&who, sizeof(who));
		if (fcin.eof()) return;
		switch (who)
		{
		case student:
			temp = new Student();
			break;
		case worker:
			temp = new Worker();
			break;
		default:
			break;
		}
		temp->Load(offset);
		temp->Show();
		cout << endl;
		fcin.seekg(offset, ios::beg);
	} while (!fcin.eof());
	fcin.close();
}

/*
		STUDENT
*/

void Student::Save()
{
	try {
		ofstream fout("f.bin", ios::binary | ios::app);
		fout.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

		People who = student;
		fout.write((char*)&who, sizeof(who));

		int length = name ? strlen(name) + 1 : 0;
		fout.write((char*)&length, sizeof(length));
		fout.write(name, length);
		fout.write((char*)&age, sizeof(age));

		length = univer ? strlen(univer) + 1 : 0;
		fout.write((char*)&length, sizeof(length));
		fout.write(univer, length);

		fout.write((char*)&countMarks, sizeof(countMarks));
		fout.write((char*)marks, sizeof(*marks) * countMarks);
		fout.close();
	}
	catch (const exception& ex)
	{
		cout << ex.what() << endl;
	}
}

void Student::Load(int& offset) {
	try {
		ifstream fcin("f.bin", ios::binary);
		fcin.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);
		fcin.seekg(offset, ios::beg);
		People who;
		fcin.read((char*)&who, sizeof(who));
		if (who == student)
		{
			delete[] name;
			int length = 0;
			fcin.read((char*)&length, sizeof(length));
			name = new char[length] {};
			fcin.read(name, length);
			fcin.read((char*)&age, sizeof(age));

			delete[] univer;
			fcin.read((char*)&length, sizeof(length));
			univer = new char [length] {};
			fcin.read(univer, length);

			delete[] marks;
			fcin.read((char*)&countMarks, sizeof(length));
			marks = new int[countMarks] {};
			fcin.read((char*)marks, sizeof(*marks) * countMarks);
			offset = fcin.tellg();
			fcin.close();
		}
		else throw exception("Student not found.");
	}
	catch (const exception& ex)
	{
		cout << ex.what() << endl;
	}
}

/*
		WORKER
*/
void Worker::Save()
{
	try {
		ofstream fout("f.bin", ios::binary | ios::app);
		fout.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);

		People who = worker;
		fout.write((char*)&who, sizeof(who));

		int length = name ? strlen(name) + 1 : 0;
		fout.write((char*)&length, sizeof(length));
		fout.write(name, length);
		fout.write((char*)&age, sizeof(age));

		length = workPlace ? strlen(workPlace) + 1 : 0;
		fout.write((char*)&length, sizeof(length));
		fout.write((char*)workPlace, length);

		fout.write((char*)&salary, sizeof(salary));
		fout.close();
	}
	catch (const exception& ex)
	{
		cout << ex.what() << endl;
	}
}

void Worker::Load(int& offset)
{
	try {
		ifstream fcin("f.bin", ios::binary);
		fcin.exceptions(ofstream::failbit | ofstream::badbit | ofstream::eofbit);
		fcin.seekg(offset, ios::beg);
		People who;
		fcin.read((char*)&who, sizeof(who));
		if (who == worker)
		{
			delete[] name;
			int length = 0;
			fcin.read((char*)&length, sizeof(length));
			name = new char[length] {};
			fcin.read(name, length);
			fcin.read((char*)&age, sizeof(age));

			fcin.read((char*)&length, sizeof(length));
			delete[] workPlace;
			workPlace = new char[length] {};
			fcin.read((char*)workPlace, length);

			fcin.read((char*)&salary, sizeof(salary));
			offset = fcin.tellg();
			fcin.close();
		}
		else throw exception("Worker not found");
	}
	catch (const exception& ex)
	{
		cout << ex.what() << endl;
	}
}