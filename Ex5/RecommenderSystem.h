//
// Created by alonn on 6/19/2020.
//


#ifndef EX5_LAST_ONE_BITCH_RECOMMENDERSYSTEM_H
#define EX5_LAST_ONE_BITCH_RECOMMENDERSYSTEM_H

#include <unordered_map>
#include <cfloat>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>
#include <functional>
#include <cmath>

#ifdef _WIN32
#include <io.h>
#define access    _access_s
#else

#include <unistd.h>

#endif

using std::vector;
using std::string;

/**
 * @brief he reccomend movies class for cinema city rishon letziyon.
 * gets as input movies and their attributes and user rankings aswell so i
 * t can detect and
 * reccomend movies personally to get the cinema back into the market!!!
 */
class RecommenderSystem
{
private:
    std::unordered_map<string, vector<double>> _movies; // movies map - has vectors of attributes
    std::unordered_map<string, vector<double>> _viewers; // viewers map - has vectors of ranks
    vector<string> _moviesList; // vector of movies by the order arranged in the viewers rankings
public:
    /**
     * @brief c'tor
     */
    RecommenderSystem();

    /**
     * @brief gets two files - the movies and the user rankings and loads the data into the
     * class fields so it can be proccesed later!.
     * @param moviesPath the path and file name to the movies
     * @param userRanks the same for the user rankings
     * @return 0 upon success or -1 if failed. it will also print an message if the file couldnt be
     * found in the given path.
     */
    int loadData(const string &moviesPath, const string &userRanks);

    /**
    * @brief reccomends a movie to the viewer by content based on the movies he watched and the way
     * he ranked them!
     * @param userName - the user we want to get the data of and reccomend accordingly
     * @return string - the movie we reccomend or an error message if the user was not found.
     */
    std::string recommendByContent(const string &userName);

    /**
     * @brief gets a user and a movie and precidts the user rating for that movie
     * @param movieName string
     * @param user string
     * @param k at most likewise movies
     * @return the prediction for the movie or -1 if user or movie are not in the database
     */
    double predictMovieScoreForUser(const string &movieName, const string &user, int k);

    /**
 * @brief reccomends a movie based on user predictions on every unwatched movie!
 * @param userName the viewer
 * @param k number of similiar movies to compare to
 * @return the most suitable movie
 */
    string recommendByCF(const string &userName, int k);

    /**
 * @brief returne the movie name based on the similiarity
 * @param index index vector for movies not watched
 * @param preferenceV preference vector of the viewer
 * @param sys the reccomender class in order to get access to the data base
 * @return the correct string
 */
    friend string
    suitableMovie(const vector<int> &index, const vector<double> &preferenceV,
                  RecommenderSystem sys);
};

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
                           index);

/**
 * @brief calculates the euclidean norm of a vector
 * @param vector1 - the vector input
 * @return double - the norm
 */
double vectorNorm(const vector<double> &vector1);

/**
 * @brief calculates the preference for a single movie
 * @param preferenceV the preference vector
 * @param attributes the attributes vector
 * @return a number double representing the suitability of this certain movie
 */
double similiarity(vector<double> preferenceV, vector<double> attributes);

/**
 * @brief vectorizes the indexes on watched movies
 * @param userRanks the user ranking for movies - vector
 * @param watched the vector to update - empty in the begining
 */
void
indexizeWatched(const vector<double> &userRanks, vector<int> &watched);

/**
 * @brief vectorizes the indexes on unwatched movies
 * @param userRanks the user ranking for movies - vector
 * @param watched the vector to update - empty in the begining
 */
void indexizeUnwatched(const vector<double> &userRanks, vector<int> &unwatched);

/**
 * @brief checks if the file exists or not
 * @param Filename string
 * @return true or false
 */
bool fileExists(const std::string &Filename);

#endif //EX5_LAST_ONE_BITCH_RECOMMENDERSYSTEM_H
