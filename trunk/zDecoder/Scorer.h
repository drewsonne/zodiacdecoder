class Scorer
{
	public:
		void ScoreKeys(string key);
		int getScore();

	private:
		int BI_GRAPHS[676];
		int TRI_GRAPHS[17576];
		int TETRA_GRAPHS[456976];
		int PENTA_GRAPHS[11881376];

		long score;

		int Scores[MAX_WORD_SCORE + 1];
		CHashTable<int> Dictionary;
		double letterFreqs[26];

		void ReadNGraphs(bool LOGWEIGHT);
		void InitializeDictionary();
		void ScoreKey(string key);
		void ReadFreqs(string FREQ_FILE);
}