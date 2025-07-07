#pragma once
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


struct timeD
{
	int day;
	int month;
	int year;
	int studyTime; // in minutes or whatever unit
};


class studytime
{
	int dateToDays(int d, int m, int y)
	{
		// Using a simple conversion formula (ignores leap years for simplicity)
		return y * 365 + m * 30 + d;
	}

	
	//function to input from file in the format
	//12-12-2023 125
	//day-month-year studyTime
	void setup(const string& filename) {
		ifstream fin(filename);
		if (!fin) {
			cout << "File not found!\n";
			return;
		}

		// 1) count lines
		count = 0;
		string line;
		while (getline(fin, line)) ++count;

		fin.clear();
		fin.seekg(0);

		times = new timeD[count];
		int i = 0;
		while (i < count && (fin >> times[i].day)) {
			char dash;
			fin >> dash
				>> times[i].month >> dash
				>> times[i].year
				>> times[i].studyTime;
			++i;
		}
		// 2) fix count so it’s the number of entries we actually parsed
		count = i;
		fin.close();
	}

	void plot(string filename = "study_plot.csv")
	{
		ofstream fout(filename);
		if (!fout)
		{
			cout << "Failed to open file for writing!" << endl;
			return;
		}

		fout << "Date,StudyTime\n";
		for (int i = 0; i < count; ++i)
		{
			fout << times[i].day << "-"
				<< times[i].month << "-"
				<< times[i].year << ","
				<< times[i].studyTime << "\n";
		}

		fout.close();
		cout << "Plot data saved to " << filename << endl;
	}


	void reset()
	{
		if (times != nullptr)
		{
			delete[] times;
			times = nullptr;
		}
		count = 0;
	}

public:
	timeD* times;
	int count = 0;
	//constructors 
	studytime()
	{
		times = nullptr;
		count = 0;

	}
	
	studytime(string filename)
	{
		times = nullptr;
		count = 0;
		setup(filename);
	}
	
	~studytime()
	{
		plot();
		reset();
	}

	void print()
	{
		for (int i = 0; i < count; ++i)
		{
			cout << times[i].day << "-"
				<< times[i].month << "-"
				<< times[i].year << " "
				<< times[i].studyTime << " minutes" << endl;
		}
	}

	//average study time regardless of days missed 
	float avgstudytime() {
		if (count == 0) return 0.0f;

		int total = 0;
		for (int i = 0; i < count; ++i)
			total += times[i].studyTime;

		// 3) force float division on the denominator
		return total / static_cast<float>(count);
	}
	//average study time considering the number of days missed 
	float avgstudytime(bool missed)
	{
		if (!missed) return avgstudytime();

		if (count == 0) return 0;

		int total = 0;
		int daysStudied = 0;

		for (int i = 0; i < count; ++i)
		{
			if (times[i].studyTime > 0)
			{
				total += times[i].studyTime;
				daysStudied++;
			}
		}

		if (daysStudied == 0) return 0;

		return static_cast<float>(total) / daysStudied;
	}


	//maximum ever study time
	int max()
	{
		if (count == 0) return 0;

		int maxVal = times[0].studyTime;
		for (int i = 1; i < count; ++i)
		{
			if (times[i].studyTime > maxVal)
				maxVal = times[i].studyTime;
		}
		return maxVal;
	}

	//minimum ever study time
	int min()
	{
		if (count == 0) return 0;

		int minVal = times[0].studyTime;
		for (int i = 1; i < count; ++i)
		{
			if (times[i].studyTime < minVal)
				minVal = times[i].studyTime;
		}
		return minVal;
	}

	//minutes to hours
	float tohours(int mins)
	{
		return static_cast<float>(mins) / 60;
	}

	//returns the number of days studied 
	int getDays()
	{
		return count;
	}

	//Returns the average frequency of studying per month 
	float avgfreqPerMonth()
	{
		if (count == 0) return 0;

		// Count unique (month, year) combinations
		int uniqueMonths = 0;
		for (int i = 0; i < count; ++i)
		{
			bool alreadyCounted = false;
			for (int j = 0; j < i; ++j)
			{
				if (times[i].month == times[j].month &&
					times[i].year == times[j].year)
				{
					alreadyCounted = true;
					break;
				}
			}
			if (!alreadyCounted)
				uniqueMonths++;
		}

		if (uniqueMonths == 0) return 0;

		return static_cast<float>(count) / uniqueMonths;
	}


	
	//total time studied 
	int totalStudyTime()
	{
		int total = 0;
		for (int i = 0; i < count; ++i)
		{
			total += times[i].studyTime;
		}
		return total;
	}
	
	
	int findByDate(int d, int m, int y)
	{
		for (int i = 0; i < count; ++i)
		{
			if (times[i].day == d && times[i].month == m && times[i].year == y)
			{
				return times[i].studyTime;
			}
		}
		return -1; // not found
	}
	
	void sortByDate()
	{
		for (int i = 0; i < count - 1; ++i)
		{
			for (int j = 0; j < count - i - 1; ++j)
			{
				int days1 = dateToDays(times[j].day, times[j].month, times[j].year);
				int days2 = dateToDays(times[j + 1].day, times[j + 1].month, times[j + 1].year);
				if (days1 > days2)
				{
					timeD temp = times[j];
					times[j] = times[j + 1];
					times[j + 1] = temp;
				}
			}
		}
	}
	

	int daysMissedInRange(int d1, int m1, int y1, int d2, int m2, int y2)
	{
		int start = dateToDays(d1, m1, y1);
		int end = dateToDays(d2, m2, y2);

		if (start > end)
			std::swap(start, end); // ensure start <= end

		// Total days in range (inclusive)
		int totalDays = end - start + 1;

		// Count how many days were studied in that range
		int studied = 0;
		for (int i = 0; i < count; ++i)
		{
			int current = dateToDays(times[i].day, times[i].month, times[i].year);
			if (current >= start && current <= end && times[i].studyTime > 0)
			{
				studied++;
			}
		}

		// Missed = total calendar days - study days
		return totalDays - studied;
	}

	//functions that gives you the average frequency of studying per month in a given range
	float avgFreqPerMonthInRange(int d1, int m1, int y1, int d2, int m2, int y2)
	{
		int start = dateToDays(d1, m1, y1);
		int end = dateToDays(d2, m2, y2);
		if (start > end)
			std::swap(start, end); // ensure start <= end
		int uniqueMonths = 0;
		for (int i = 0; i < count; ++i)
		{
			int current = dateToDays(times[i].day, times[i].month, times[i].year);
			if (current >= start && current <= end)
			{
				bool alreadyCounted = false;
				for (int j = 0; j < i; ++j)
				{
					if (times[i].month == times[j].month && times[i].year == times[j].year)
					{
						alreadyCounted = true;
						break;
					}
				}
				if (!alreadyCounted)
					uniqueMonths++;
			}
		}
		if (uniqueMonths == 0) return 0;
		int totalStudyDays = 0;
		for (int i = 0; i < count; ++i)
		{
			int current = dateToDays(times[i].day, times[i].month, times[i].year);
			if (current >= start && current <= end)
			{
				totalStudyDays++;
			}
		}
		return static_cast<float>(totalStudyDays) / uniqueMonths;
	}
};