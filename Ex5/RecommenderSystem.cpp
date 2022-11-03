//
// Created by alonn on 6/19/2020.
//
#include "RecommenderSystem.h"


#define FAIL -1
#define FILESUCCESS 0
#define ZERO 0
#define FILEMSG "Unable to open file "
#define EMPTY "NA"
#define UNFOUND "USER NOT FOUND"
#define SQUARE 2

// Recommend class functions

/**
     * @brief gets two files - the movies and the user rankings and loads the data into the
     * class fields so it can be proccesed later!.
     * @param moviesPath the path and file name to the movies
     * @param userRanks the same for the user rankings
     * @return 0 upon success or -1 if failed. it will also print an message if the file couldn't be
     * found in the given path.
     */
int RecommenderSystem::loadData(const string &moviesPath, const string &userRanks)
{
    if (!fileExists(moviesPath)) // check if file exists
    {
        std::cout << FILEMSG << moviesPath << std::endl;
        return FAIL;
    }
    if (!fileExists(userRanks)) // check if file exists
    {
        std::cout << FILEMSG << userRanks << std::endl;
        return FAIL;
    }
    std::ifstream fileOne(moviesPath); //gets the file stream in order to read
    std::ifstream fileTwo(userRanks); // same
    if (fileOne.is_open()) //load data section for file number one - the movies attributes
    {
        string line;
        while (getline(fileOne, line))
        {
            bool firstWord = true;
            double number;
            string element, movieName;
            std::istringstream iss(line, std::istringstream::in); //for whitespace parsing
            vector<double> movieAttribues;
            while (iss >> element) //element is a "word" in each line
            {
                if (firstWord)
                {
                    firstWord = false;
                    movieName = element;
                    continue;
                }
                number = std::stod(element); //conversion to double
                movieAttribues.emplace_back(number);
            }
            _movies.insert(std::make_pair(movieName, movieAttribues)); // add the movie to map
        }
    }
    else
    {
        return FAIL;
    }
    if (fileTwo.is_open()) //all comments from previous read are the same so no need to duplicate
    {
        string line;
        bool firstLine = true; //first line is different - we create movie vector for correct order
        while (getline(fileTwo, line))
        {
            double number;
            bool firstWord = true;
            string element, viewerName;
            std::istringstream iss(line, std::istringstream::in);
            vector<double> viewerRanks;
            if (firstLine)
            {
                while (iss >> element)
                {
                    _moviesList.emplace_back(element); //add the movie as written in the file.
                }
                firstLine = false;
                continue;
            }
            while (iss >> element)
            {
                if (firstWord)
                {
                    viewerName = element;
                    firstWord = false;
                    continue;
                }
                if (element == EMPTY)
                {
                    viewerRanks.emplace_back(ZERO);
                    continue;
                }
                number = std::stod(element);
                viewerRanks.emplace_back(number);
            }
            _viewers.insert(std::make_pair(viewerName, viewerRanks));
        }
    }
    else
    {
        return FAIL;
    }
    return FILESUCCESS;
}


/**
 * @brief reccomends a movie to the viewer by content based on the movies he watched and the way
 * he ranked them!
 * @param userName - the user we want to get the data of and reccomend accordingly
 * @return string - the movie we reccomend or an error message if the user was not found.
 */
string RecommenderSystem::recommendByContent(const string &userName)
{
    if (_viewers.count(userName) != 1)
    {
        return UNFOUND;
    }
    vector<double> ranks = _viewers.at(userName);
    vector<vector<double>> moviesRankedAttributes;
    vector<int> indexes, antiIndexes; // we did not call indexizeWatched since the loop does more
    // actions besides that single one
    int aboveZero = 0;
    double sum = 0, average = 0;
    for (unsigned int i = 0; i < ranks.size(); i++) // collect ranks for average + create index
        // helpers
        // and ranked movies vector for preference vector calculations
    {
        if (ranks[i] > ZERO)
        {
            sum += ranks[i];
            aboveZero++;
            moviesRankedAttributes.emplace_back(_movies.at(_moviesList[i])); //the movies we need
            // for the preference vector calculation
            indexes.emplace_back(i); // indexes we have movies viewed at
        }
        else
        {
            antiIndexes.emplace_back(i); // indexes we dont have movies viewed at
        }
    }
    average = sum / aboveZero;
    for (double &rank : ranks) // normalize rankings
    {
        if (rank > ZERO)
        {
            rank = rank - average;
        }
    }
    vector<double> preference = preferenceV(ranks, moviesRankedAttributes, indexes);
    return suitableMovie(antiIndexes, preference, *this);
}

/**
     * @brief gets a user and a movie and precidts the user rating for that movie
     * @param movieName string
     * @param user string
     * @param k at most likewise movies
     * @return the prediction for the movie or -1 if user or movie are not in the database
     */
double
RecommenderSystem::predictMovieScoreForUser(const string &movieName, const string &user, int k)
{
    if (_movies.count(movieName) != 1 || _viewers.count(user) != 1)
    {
        return FAIL;
    }
    double current, mone = 0, mechane = 0;
    vector<double> userRanks = _viewers.at(user);
    vector<double> movieAttributes = _movies.at(movieName);
    vector<int> watched;
    vector<std::pair<double, int>> similiarities;
    indexizeWatched(userRanks, watched);
    for (int i : watched)
    {
        current = similiarity(movieAttributes, _movies.at(_moviesList[i]));
        similiarities.emplace_back(std::make_pair(current, i));
    }
    std::sort(similiarities.begin(), similiarities.end());
    similiarities.erase(similiarities.begin(), similiarities.begin() + similiarities.size() - k);
    for (auto &similiaritie : similiarities)
    {
        mone += (similiaritie.first * userRanks[similiaritie.second]);
        mechane += similiaritie.first;
    }
    return mone / mechane;
}


/**
 * @brief reccomends a movie based on user predictions on every unwatched movie!
 * @param userName the viewer
 * @param k number of similiar movies to compare to
 * @return the most suitable movie
 */
string RecommenderSystem::recommendByCF(const string &userName, int k)
{
    if (_viewers.count(userName) != 1)
    {
        return UNFOUND;
    }
    vector<int> unwatched;
    string movie;
    double pred = -DBL_MAX, cur = 0;
    vector<std::pair<double, string>> candidates;
    indexizeUnwatched(_viewers.at(userName), unwatched);
    for (int i : unwatched)
    {
        double prediction = predictMovieScoreForUser(_moviesList[i], userName, k);
        candidates.emplace_back(std::make_pair(prediction, _moviesList[i]));
    }
    for (auto &candidate : candidates)
    {
        cur = candidate.first;
        if (cur > pred)
        {
            pred = cur;
            movie = candidate.second;
        }
    }
    return movie;
//    const auto ptr = std::minmax_element(candidates.begin(), candidates.end());
//    return ptr.second->second;
}

/**
     * @brief c'tor
     */
RecommenderSystem::RecommenderSystem()
{
}

//other functions + friend function
/**
 * @brief checks if the file exists or not
 * @param Filename string
 * @return true or false
 */
bool fileExists(const std::string &Filename)
{
    return access(Filename.c_str(), 0) == 0;
}

/**
 * @brief this function creates the preference vector for the viewer according to the movies
 * the viewer viewed and to the normalized rank vector
 * @param normalized the normalized rank vector after deduction of the average
 * @param ranks vector of vectors containg the attributes of the movies the viewer has watched
 * @param index vector of indexes in order to have easy direct access to normalized elements
 * in case we recieve zero in any of them.
 * @return the preference vector for the viewer according to attributes
 */
vector<double> preferenceV(vector<double> normalized, vector<vector<double>> ranks, vector<int>
                           index)
{
    vector<double> preference;
    for (unsigned int i = 0; i < ranks[ZERO].size(); i++)
    {
        preference.emplace_back(ZERO);
    }
    for (unsigned int i = 0; i < index.size(); i++)
    {
        for (unsigned int j = 0; j < ranks[i].size(); j++)
        {
            ranks[i][j] = normalized[index[i]] * ranks[i][j];
            preference[j] += ranks[i][j];
        }
    }
    for (unsigned int i =0; i<preference.size();i++)
    {
        std::cout << "index :" << i << " value: " << preference[i]<< std::endl;
    }
    return preference;
}

/**
 * @brief returne the movie name based on the similiarity
 * @param index index vector for movies not watched
 * @param preferenceV preference vector of the viewer
 * @param sys the reccomender class in order to get access to the data base
 * @return the correct string
 */
string
suitableMovie(const vector<int> &index, const vector<double> &preferenceV, RecommenderSystem sys)
{
    string chosenMovie;
    double affinity = -DBL_MAX, current;
    for (int i : index)
    {
        current = similiarity(preferenceV, sys._movies.at(sys._moviesList[i]));
        if (current > affinity)
        {
            chosenMovie = sys._moviesList[i];
            affinity = current;
        }
    }
    return chosenMovie;
}

/**
 * @brief calculates the euclidean norm of a vector
 * @param vector1 - the vector input
 * @return double - the norm
 */
double vectorNorm(const vector<double> &vector1)
{
    double sum = 0;
    for (double i : vector1)
    {
        sum += pow(i, SQUARE);
    }
    return sqrt(sum);
}

/**
 * @brief calculates the preference for a single movie
 * @param preferenceV the preference vector or the second attributes vector
 * @param attributes the attributes vector
 * @return a number double representing the suitability of this certain movie
 */
double similiarity(vector<double> preferenceV, vector<double> attributes)
{
    double sum = 0;
    for (unsigned int i = 0; i < preferenceV.size(); i++)
    {
        sum += preferenceV[i] * attributes[i];
    }
    double normOne = vectorNorm(preferenceV);
    double normTwo = vectorNorm(attributes);
    return (sum / (normOne * normTwo));
}


/**
 * @brief vectorizes the indexes on watched movies
 * @param userRanks the user ranking for movies - vector
 * @param watched the vector to update - empty in the begining
 */
void indexizeWatched(const vector<double> &userRanks, vector<int> &watched)
{
    for (unsigned int i = 0; i < userRanks.size(); i++)
    {
        if (userRanks[i] > ZERO)
        {
            watched.emplace_back(i);
        }
    }
}

/**
 * @brief vectorizes the indexes on unwatched movies
 * @param userRanks the user ranking for movies - vector
 * @param watched the vector to update - empty in the begining
 */
void indexizeUnwatched(const vector<double> &userRanks, vector<int> &unwatched)
{
    for (unsigned int i = 0; i < userRanks.size(); i++)
    {
        if (userRanks[i] > ZERO)
        {
            continue;
        }
        unwatched.emplace_back(i);
    }
}
