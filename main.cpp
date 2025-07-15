#include <iostream>
#include <cstdlib>
#include "time.h" 
using namespace std;

int main()
{
	cout << "Bonjour..." << endl;
	cout << "===== Study Time Summary =====" << endl;

	studytime t("data.txt");

	//cout << "\n--- Study Sessions ---\n";
	//t.print();

	cout << "\n--- Statistics ---" << endl;
	cout << "Average Frequency per Month: " << t.avgfreqPerMonth() << " days/month" << endl;
	cout << "Average Study Time (All): " << t.avgstudytime() << " mins" << endl;
	cout << "Total Days Studied: " << t.getDays() << endl;
	cout << "Total Study Time: " << t.totalStudyTime() << " mins" << endl;
	cout << "Maximum Study Time in a Day: " << t.max() << " mins" << endl;
	cout << "Minimum Study Time in a Day: " << t.min() << " mins" << endl;

	cout << "\n--- Missed Days ---" << endl;
	cout << "Missed Days Between 19-8-2024 and 12-1-2025: "
		<< t.daysMissedInRange(19, 8, 2024, 12, 1, 2025) << endl;

	cout << "Average Study Time in Range (19-8-2024 to 12-1-2025): "
		<< t.avgStudyTimeInRange(19, 8, 2024, 12, 1, 2025) << " mins" << endl;

	cout << "\n--- Search ---" << endl;
	cout << "Study Time on 20-12-2023: " << t.findByDate(20, 12, 2023) << " mins" << endl;

	cout << "Average Frequency in range: " << t.avgFreqPerMonthInRange(1,9,2024,29,9,2024) << " for Spetmeber 2024 " << endl;

	cout << "\n===== End of Report =====\n";

	return 0;
}
