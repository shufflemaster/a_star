// PerfCounter.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "PerfCounter.h"

float addf(float a, float b) {
	return a + b;
}

double addd(double a, double b) {
	return a + b;
}

static void float_test(int cnt)
{
	float a = 0.f;
	float b = 0.f;
	float c = 0.f;

	CPerfCounter perfy;

	perfy.start();
	for (int i = 0; i < cnt; i++) {
		c += addf(a, b);
		a += 1.f;
		b += 1.f;
	}
	perfy.stop();

	perfy.dump(cnt);
}

static void double_test(int cnt)
{
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;

	CPerfCounter perfy;

	perfy.start();
	for (int i = 0; i < cnt; i++) {
		c += addd(a, b);
		a += 1.0;
		b += 1.0;
	}
	perfy.stop();

	perfy.dump(cnt);
}



int main()
{
	int cnt;
	cout << "Max Count?" << endl;
	cin >> cnt;

	cout << "float test" << endl;
	float_test(cnt);

	cout << endl << "double test" << endl;
	double_test(cnt);

	cin >> cnt;

    return 0;
}

