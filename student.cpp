#include "student.h"

    student::student() { setId(0); discipline_list.clear();}

    void student::print() const
    {
        std::cout << "ID: " << getid() << std::endl;
        std::cout << "Name: " << getname() << std::endl;
        std::cout << "Chair: " << getchair() << std::endl;
        std::cout << "Semester: " << getsemester() << std::endl;
        for (auto i : discipline_list)
        {
            std::cout << "\t\tdiscipline: " << i.name << " - " << i.mark << std::endl;

        }
    }

    bool student::isNegative ()
    {
        for (auto i : discipline_list)
        {
            if (i.mark < 3) {return true;}
        }
        return false;
    }

    void student::add_discipline() 
    {
        bool isOk = true;
        do {
            discipline ds;
            std::cout << "Enter discipline name, type exit - for break input or leave name blank for skip" << std::endl;
            std::getline (std::cin, ds.name);
            if (ds.name == "exit") {return;}
            if (ds.name.size() == 0) 
            {
                break;
            }
            std::cout << "Enter disciplene mark, type exit or leave in blank for defaul value (2)";
            std::string temp = "";
            std::getline (std::cin, temp);
            if (temp == "exit") {return;}
            try {
                if (temp.size() == 0) {ds.mark = 2;}
                else {ds.mark = stoi(temp);}

                discipline_list.push_back(ds);
                std::cout << "Do you want to enter new discipline? (y - yes, n - no)" << std::endl;
                char ch2;
                bool isOk2 = true;
                do
                {
                    ch2 = std::getchar();
                    std::cin.ignore(1);
                    switch (ch2)
                    {
                    case 'y':
                        isOk = false;
                        break;
                    case 'n':
                        isOk = true;
                        break;
                    
                    default:
                        isOk2 = false;
                        break;
                    }
                } while (!isOk2);
                
            }
            catch (std::exception exc)
            {
                std::cout << "Wrong value" << std::endl;
                isOk = false;
            }
        } while (!isOk);

    } 

    void student::edit_discipline () 
    {
       char ch;
       bool isOk = false;
       for (int i = 0; i < discipline_list.size(); i++)
       {
            discipline_list[i].print();
            do {
                std::cout << "Do you want edit this discipline? (y - yes, n - no) " << std::endl;
                ch = std::getchar();
                std::cin.ignore(1);

                switch (ch)
                {
                    case 'y':
                        {
                            std::string temp = ""; 
                            std::cout << "Enter NEW discipline name or type exit" << std::endl;
                            std::getline(std::cin, temp);
                            if (temp.substr(0, 4) == "exit") {return;}
                            discipline_list[i].name = temp;
                            temp = "";
                            //------------------  
                            std::cout << "Enter NEW discipline mark or type exit" << std::endl;
                            std::getline(std::cin, temp);
                            if (temp.substr(0, 4) == "exit") {return;}
                            discipline_list[i].mark = stoi(temp);
                            temp = "";
                            //------------------                                    
                            
                            
                            isOk = true;
                            break;
                        }
                    case 'n':
                        {
                            isOk = true;
                            break;
                        }
                    default:
                        isOk = false;
                        break;
                }

            } while (!isOk);
        } 
    }


    void student::del(int indx) 
    {
        discipline_list.erase(discipline_list.begin() + indx);
    }

    std::string student::serialize ()  
    {
        std::string temp = "0";
        temp = temp + "[\""+ toString(getid()) + "\"" + ",\"" +getname()+"\"" + ",\"" + getchair()+ "\"" + ",\"" + toString(getsemester()) + "\""+ "[";
        for (auto i : discipline_list)
        {
            temp = temp + "[\""+ i.name + "\",\"" + toString(i.mark) + "\"],";
        } 
        temp = temp + "]]";
        return temp;
    }

    void student::findBegEnd (std::string & str, int & beg, int & end)
    {
        int pos = end + 1;
        beg = str.find("\"", pos);
        pos = beg + 1;
        end = str.find("\"", pos); 
    }

    std::string student::getValue (std::string str, int beg, int end)
    {
        beg++;
        return str.substr(beg, end - beg);
    }

    void student::deserialize (std::string str) 
    {
        std::string temp = "";
        int pos_b = 0;
        int pos_e = 0;
        int pos = 0;
        int size = str.size();
        discipline_list.clear();
        discipline ds;
        for (int i = 0; i < str.size()-4; i++)
        {
            findBegEnd (str, pos_b, pos_e);
            
            if (pos == 0)         {setId(stoi(getValue(str, pos_b, pos_e))); pos++; i = pos_e + 1;}
            else if (pos == 1)    {setname(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
            else if (pos == 2)    {setchair(getValue(str, pos_b, pos_e)); pos++; i = pos_e + 1;}
            else if (pos == 3)    
            {
                setsemestr(stoi(getValue(str, pos_b, pos_e)));
                pos++; i = pos_e + 1;
            }
            else if (pos == 4)    
            {
                ds.name = getValue(str, pos_b, pos_e); 
                findBegEnd (str, pos_b, pos_e);
                ds.mark = stoi(getValue(str, pos_b, pos_e));   
                i = pos_e + 1; 
                discipline_list.push_back(ds);            
            }

        }
    } 


