#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <opencv2/opencv.hpp>
#define MAX_MEMORY 100 //Mb

//image memory class
class Images
{
public:
    std::map<std::string,cv::Mat> img_data;
};

//check for image name in memory
bool checkName(Images &image,const std::string &name)
{
    auto it = image.img_data.find(name);
    if(it == image.img_data.end())
    {
    	return false;
    }
    return true;	
}    

//check saved name with ending .jpg
//so string ".jpg" contains 4 chars, that's why we've got magic number 4!!!!!!
//if not checking this openCV won't save the image!!!!
bool checkFormat(const std::string name)
{
    std::string format;
    for(size_t k = name.size() - 4; k != name.size(); k++)
    {
        format.push_back(name[k]);
    }
    if(format != ".jpg")
        return false;
    return true;
}

//get input command
std::vector<std::string> getCommand()
{
    std::cout << "\n\nenter command ->";
    std::string input;
    
    //devide input string into string parts to get commands and parameters
    std::cin.clear();
    getline(std::cin,input);
    int count = 0; //count parameters
    int min = 0; //first symbol of parameter
    int max = 0; //last symbol of parameter
    std::vector<std::string> command_line = {""}; //vector to store command and parameters
    
    for(size_t k = 0; k != input.size(); k++)
    {
        //get command
        if(count == 0)
        {
            if(input[k] != ' ')
            {
                command_line[0].push_back(input[k]);
                continue;
            }
            else
            {
                count++;
                continue;
            }
        }
        //get all parameters
        else
        {
            if(min == 0 && input[k] != ' ')
            {
                min = k;
            }
                
                
            if(min > 0 && (input[k + 1] == ' ' || k + 1 >= input.size()))
            {
                max = k + 1;
            }
            
            //write parameter to memory
            if(max > 0)
            {
                std::string par;
                for(int j = min; j != max; j++)
                {
                    if(input[j] == ' ')
                        continue;
                    par.push_back(input[j]);
                }
                command_line.push_back(par);
                min = max;
                max = 0;
            }
        } 
    }
    return command_line;
}

//convert string to int
int convertString(std::string &name)
{
    std::istringstream ss(name);
    unsigned int blur;
    ss >> blur;
    return static_cast<int>(blur);
}

bool menu(Images &image)
{   
    //initiate map with commands
    std::map<std::string, int> command;
    command["load"] = 1;
    command["ld"] = 1;
    command["store"] = 2;
    command["s"] = 2;
    command["blur"] = 3;
    command["b"] = 3;
    command["help"] = 4;
    command["h"] = 4;
    command["resize"] = 5;
    command["r"] = 5;
    command["view"] = 6;
    command["v"] = 6;
    command["exit"] = 7;
    command["quit"] = 7;
    command["q"] = 7;
    
    std::map<std::string, int> :: iterator it;
    
    std::vector<std::string> command_line;
    int choice = 0;
    do
    {   
        command_line = getCommand();
        it = command.find(command_line[0]);
        if(it == command.end())
            std::cout << "\nInvalid input!!!!";
        else
        {
            choice = it->second;
            std::cin.clear();
            break;
        }
        
    }while(true);  
    
    
    switch(choice)
    {
        //load
        case(1) : 
        {
            //check for command
            if(command_line.size() != 3)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            
            cv::Mat img = cv::imread(command_line[2]);
            
            //check for image path
            if(img.empty())
            {
                std::cout << "\nInvalid path to image!!!!" << " " << command_line[2];
                break;
            }
            
            //check for image in memory
            if(checkName(image,command_line[1]))
            {
            	std::cout << "Invalid name! Operation aborted!";
                break;
            }
            
            //insert image to memeory
            image.img_data.insert(std::pair<std::string,cv::Mat>(command_line[1],img));
            std::cout << "\nLoaded";
            break;
        }
        //store
        case(2) : 
        {
            //check for command
            if(command_line.size() != 3)
            {
                std::cout << "\nInvalid command!";
                break;
            }
                
            //check format
            if(!checkFormat(command_line[2]))
            {
                std::cout << "\nInvalid format! Image name must ends with .jpg";
                break;
            }
            
            //check for image in memory
            if(!checkName(image,command_line[1]))
            {
            	std::cout << "Invalid name! Operation aborted!";
                break;
            }
                
            cv::Mat output_img = image.img_data.find(command_line[1])->second;
            
            //save image
            cv::imwrite(command_line[2],output_img);  
            std::cout << "\nStored";
            break;
        }
        //blur
        case(3) : 
        {
            //check for command
            if(command_line.size() != 4)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            
            //convert string to int
            int blur_size = convertString(command_line[3]);
            
            //check for blur size
            if(blur_size <= 0)
                break;
            
            //check for image in memory
            if(!checkName(image,command_line[1]))
            {
            	std::cout << "Invalid name! Operation aborted!";
                break;
            }
            
            //make blur image
            cv::Mat output_img;
            cv::blur(image.img_data.find(command_line[1])->second,output_img,cv::Size(blur_size,blur_size));
            
            //write blured image to memory
            image.img_data.insert(std::pair<std::string,cv::Mat>(command_line[2],output_img));
            std::cout << "\nBlured";
            break;
        }
        //help
        case(4) : 
        {
            //check for command
            if(command_line.size() != 1)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            
            std::cout << "This application works only with JPEG format images. Help commands:\n"
            << "\nload, ld <image_name> <path_to_image> - load image to memory"
            << "\nstore, s <image_name> <path_to_directory> - save image"
            << "\nblur <image_name> <image_name_blured> <5> - make new blured image from current image"
            << "\nresize <image_name> <image_name_resized> <1280> <720> - "
            << "resize current image to image 1280 720"
            << "\nhelp, h - show commands"
            << "\nview, v - show all stored in memory images"
            << "\nexit,quit,q - exit application";          
            break;
        }
        //resize
        case(5) : 
        {
            //check for command
            if(command_line.size() != 5)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            
            //check for image in memory
            if(!checkName(image,command_line[1]))
            {
            	std::cout << "Invalid name! Operation aborted!";
                break;
            }
            
            //convert string to int
            int width = convertString(command_line[3]);
            int height = convertString(command_line[4]);
            cv::Size size_new(width,height);
            
            //resize image
            cv::Mat output_img;
            cv::resize(image.img_data.find(command_line[1])->second,output_img,size_new);
            
            //save image in memory
            image.img_data.insert(std::pair<std::string,cv::Mat>(command_line[2],output_img));
            std::cout << "\nResized";
            break;
        }
        //view
        case(6) : 
        {
            //check for command
            if(command_line.size() != 1)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            
            //check memory for images
            if(image.img_data.empty())
            {
                std::cout << "\nMemory is empty!!!!";
                break;
            }
            
            for(auto elem : image.img_data)
            {
                std::cout << "\n" << elem.first << " " << elem.second.size().width << "x"
                << elem.second.size().height;
            }
            
            std::cout << "\n\nViewed";
            break;
        }
        //exit
        case(7) : 
        {
            //check for command
            if(command_line.size() != 1)
            {
                std::cout << "\nInvalid command!";
                break;
            }
            return false;
            break;
        }
        default: break;
    }
    return true;
}


int main()
{
    std::cout << "Welcome to image processor application. It can save, load, resize and blur \
JPEG images. \nTo see available commands type \"help\" or \"h\"\n\nCreated by E-Protocol";
    
    Images image; //create memory
    std::cout << std::fixed << std::setprecision(2); // round double memory output info
    while(true)
    {
        if(!menu(image))
            break;
    }
	return 0;
}
