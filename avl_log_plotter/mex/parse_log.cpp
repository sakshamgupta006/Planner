//==============================================================================
// Autonomous Vehicle Library
//
// Description: Parses a log file's data entries into separate MATLAB-readable 
//              files based on the data entry's custom tag.
//
// Author:      babiggs, stkrauss
//==============================================================================

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "mex.h"

// Structure representing the elements of a log file
typedef struct LogFile
{

	size_t num_lines = 0;
	std::vector<std::string> timestamps;
	std::vector<std::string> levels;
	std::vector<std::string> node_names;
	std::vector<std::string> custom_tags;
	std::vector<std::string> messages;

} LogFile;

//==============================================================================
//                              CLASS DEFINITION
//==============================================================================

class LogParser
{

public:

    //--------------------------------------------------------------------------
    // Name:        LogParser constructor
    //--------------------------------------------------------------------------
    LogParser(std::string filepath) : filepath(filepath)
    {

        char folder_delimiter = '/';
        #ifdef _WIN32
           folder_delimiter = '\\';
        #endif
        
        // Parse the name of the log file from the filepath
           
        // Locate the last instance of the folder delimiter characters,
        // and take all characters after it to get the log file name with
        // the file extension
        std::size_t last_delimiter_pos = filepath.find_last_of(folder_delimiter);
        log_file_name = filepath.substr(last_delimiter_pos+1, std::string::npos);
        log_file_folder = filepath.substr(0, last_delimiter_pos+1);
        
        // Find the . in the file name and take the characters from the 
        // start to the . to get the file name without extension
        last_delimiter_pos = log_file_name.find_last_of(".");
        log_file_name = log_file_name.substr(0, last_delimiter_pos);
        
		// Open the log file to be read
        file.open(filepath);
        if (!file.is_open())
        {
            mexPrintf("Failed to open file %s\r\n", filepath.c_str());
            mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
        }

    }

public:

    // Full filepath to the log file to be parsed
    std::string filepath;
    
    // Name of the log file (without extension and the path to the folder
    // containing it
    std::string log_file_name;
    std::string log_file_folder;

    // Input file stream to read the file
    std::ifstream file;

	// LogFile struct to contain the log file's data
	LogFile log_file;

public:

    //--------------------------------------------------------------------------
    // Name:        split
    // Description: Splits a string into a vector of strings according the the
    //              given delimeter string. The delimeter is removed from the
    //              strings.
    // Arguments:   - str: string to be split
    //              - delim: split delimeter
    // Returns:     a vector of split strings.
    //--------------------------------------------------------------------------
    std::vector<std::string> split(const std::string& str, const std::string& delim)
    {

        std::vector<std::string> substrings;
        size_t pos = 0;
        size_t prev_pos = 0;

        // Loop through the string looking for the delimeter
        do
        {

            pos = str.find(delim, prev_pos);
            if (pos == std::string::npos)
            {
                pos = str.length();
            }

            // Get the substring between the previous delimeter and the new one
            std::string token = str.substr(prev_pos, pos - prev_pos);
            substrings.push_back(token);

            prev_pos = pos + delim.length();

        } while (pos < str.length() && prev_pos < str.length());

        return substrings;

    }

	//--------------------------------------------------------------------------
	// Name:        strip
	// Description: Removes all instances of a character from a string.
	// Arguments:   - str: string from which to strip character instances
	//              - character: character to remove from the string
	// Returns:     The string with all instances of the character removed.
	//--------------------------------------------------------------------------
    std::string strip(std::string str, char character)
    {
        std::string output;
        for (size_t i = 0; i < str.size(); i++)
            if (str[i] != character)
                output += str[i];
        return output;
    }

	//--------------------------------------------------------------------------
	// Name:        has_element
	// Description: Checks whether a vector of strings contains an element
	//              matching the given element.
	// Arguments:   - vec: Vector to search for element instances
	//              - element: element to check for
	// Returns:     True if the vector contains one or more of the element,
	//              false if it does not.
	//--------------------------------------------------------------------------
    bool has_element(std::vector<std::string> vec, std::string element)
    {
        bool matches = false;
        for(size_t i = 0; i < vec.size(); i++)
            if(!element.compare(vec.at(i)))
                matches = true;
        return matches;
    }

	//--------------------------------------------------------------------------
	// Name:        remove_duplicates
	// Description: Gets a vector of only unique elements from a given vector,
	//              removing any duplicates.
	// Arguments:   - vec: Vector to remove duplicates from
	// Returns:     Vector with duplicate elements removed.
	//--------------------------------------------------------------------------
	std::vector<std::string> remove_duplicates(std::vector<std::string> vec)
	{
		std::vector<std::string> unique_elements;
		for (std::string element: vec)
			if (!has_element(unique_elements, element) && element != "")
				unique_elements.push_back(element);
		return unique_elements;
	}

    //--------------------------------------------------------------------------
    // Name:        split
    // Description: Splits a string into a vector of strings according the the
    //              given delimeter string. The delimeter is removed from the
    //              strings.
    // Arguments:   - str: string to be split
    //              - delim: split delimeter
    // Returns:     a vector of split strings.
    //--------------------------------------------------------------------------
    void read_log_file()
    {

        // Read lines from the file until no lines remain
		std::string line;
        while(std::getline(file, line))
        {

            try
            {

                // Remove any newline or carraige return characters from the line
                line = strip(line, '\r');
                line = strip(line, '\n');

                // Find the last occurance of a ] character which signifies the end
                // of a tag. All characters before this are tags, and all characters
                // after are the message
                std::size_t last_tag = line.find_last_of("]");

                // If the last tag index is npos, no ] character was found, and
                // the line does not contain any tags. This line is therefore 
                // invalid and can be ignored
                if (last_tag == std::string::npos)
                {
                    mexPrintf("    Ignoring invalid log line: %s \n", line.c_str());
                    continue;
                }

                // Split the line into a tags string and a message string. Add two
                // to the message start index to skip the last ] and the space
                std::string tags_string = line.substr(0, last_tag+1);
                std::string message_string = line.substr(last_tag+2);

                // Split the tags string by spaces to separate the line's tags,
                // and get the timestamp, log level, and node name from the tags.
			    // Remove the brackets from the tags
			    tags_string = strip(tags_string, '[');
			    tags_string = strip(tags_string, ']');
                std::vector<std::string> line_tags = split(tags_string, " ");

			    // Add the current line to the log file struct
			    log_file.timestamps.push_back(line_tags.at(0));
			    log_file.levels.push_back(line_tags.at(1));
			    log_file.node_names.push_back(line_tags.at(2));
			    if (line_tags.size() > 3)
				    log_file.custom_tags.push_back(line_tags.at(3));
			    else
				    log_file.custom_tags.push_back("");
			    log_file.messages.push_back(message_string);
			    log_file.num_lines++;

			}
			catch (const std::exception& ex)
			{
				mexPrintf("    Ignoring invalid log line: %s \n", line.c_str());
                mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
			}

        }

		// Close the file since we're done reading from it
        file.close();
        
        mexPrintf("    Successfully read file \n");

		// Get a list of the logging levels and custom tags that are present in
		// the file
		std::vector<std::string> unique_log_levels = remove_duplicates(log_file.levels);
		std::vector<std::string> unique_custom_tags = remove_duplicates(log_file.custom_tags);

		// Loop through the custom tags, creating a new file for each one, and
		// writing the messages with that tag to it
		for (std::string tag: unique_custom_tags)
		{

			// Create a file for the custom tag
			std::string output_filepath = log_file_folder + log_file_name + "." + tag + ".dat";
			std::ofstream output_file(output_filepath);
			mexPrintf("    Writing to file %s \n", output_filepath.c_str());
            mexCallMATLAB(0, NULL, 0, NULL, "drawnow");

			if (output_file.is_open())
			{
                
				// Loop through the log file messages. If the message logging
				// level is data, and it has the tag we're looking for, write
				// the timestamp and message to the file
				for (size_t i = 0; i < log_file.num_lines; i++)
					if (log_file.levels.at(i) == "DAT" && log_file.custom_tags.at(i) == tag)
						output_file << log_file.timestamps.at(i) << " " << log_file.messages.at(i) << std::endl;

				// Close the file since we're done writing to it
				output_file.close();

			}
			else
			{
				mexPrintf("    Failed to open output file %s \n", output_filepath.c_str());
                mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
			}

		}

    }

};

//==============================================================================
//                              MEX FUNCTION
//==============================================================================
void mexFunction( int nlhs, mxArray *plhs[],
                  int nrhs, const mxArray *prhs[])
{

    // Check for proper number of input and output arguments
    if(nrhs!=1)
        mexErrMsgIdAndTxt( "MATLAB:revord:invalidNumInputs", "One input required.");
    else if(nlhs > 1)
        mexErrMsgIdAndTxt( "MATLAB:revord:maxlhs", "Too many output arguments.");

    // Input argument must be a string
    if (mxIsChar(prhs[0]) != 1)
        mexErrMsgIdAndTxt( "MATLAB:revord:inputNotString", "Input 1 must be a string.");

    // Get the input argument as a C string
    char* filepath_buf = mxArrayToString(prhs[0]);

    // Ensure that the input argument was converted to a string properly
    if(filepath_buf == NULL)
        mexErrMsgIdAndTxt( "MATLAB:revord:conversionFailed", "Could not convert input to string.");

    // Create a C++ string from the C string
    std::string filepath(filepath_buf);
    mxFree(filepath_buf);

    // Run the log parser
    mexPrintf("\n================================================================================\n\n");
	mexPrintf("Parsing %s \n", filepath.c_str());
    mexCallMATLAB(0, NULL, 0, NULL, "drawnow");
    LogParser parser(filepath);
    parser.read_log_file();
	mexPrintf("Finished parsing %s \n", filepath.c_str());
    mexCallMATLAB(0, NULL, 0, NULL, "drawnow");

    return;

}
