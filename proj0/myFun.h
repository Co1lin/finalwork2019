#ifndef MYFUN_H
#define MYFUN_H
#include "mainwindow.h"
#include <sstream>
#include <QPainter>
#include <QPen>
#include <QFile>
#include <QDir>
using namespace std;

static const double pi = 3.141592653589792;

struct  Turtle
{
    double x, y, angle;
    bool not_cloak;

    void cloak()
    {
        not_cloak = 0;
    }
    void appear()
    {
        not_cloak = 1;
    }

    void turn(const double &delta)
    {
        angle += delta;
        while (angle >= 360)
            angle -= 360;
    }

    void move(const double &distance, QPainter &painter)
    {
        double xx = x + distance * cos(pi * angle / 180);
        double yy = y + distance * sin(pi * angle / 180);
        QPointF p1(x, y), p2(xx, yy);
        if (not_cloak)
            painter.drawLine(p1, p2);
            //painter.drawLine(x, y, xx, yy);
        x = xx;
        y = yy;
    }
};

struct myCmd
{
    int token;
    vector<string> data;
};

struct myOpt
{
    int id;
    int var_num;
};

struct myVar
{
    string name;
    double var;
};

extern map<string, myOpt> tokens;
extern vector<myVar> vars;
extern Turtle turtle;
extern ifstream infile;

double s2num(const string &s);  //convert a string(number string or variate string) to a double type number
int ini_list(); //initialize the token map
int readHead(int &width, int &height, int &r, int &g, int &b, double &xpos, double &ypos);
int myExe(const myCmd &cmd, QPainter &painter, vector<myCmd>::iterator &in_block_cmd);
int readLine(myCmd &cmd);
void errorOccurred();

struct codeBlock
{
    vector<myCmd> cmds;
    int type;
    string name;    //function name
    int para_num;
    int cmd_num;

    codeBlock(const int &_type):type(_type) {}

    int exec(QPainter &painter);
};
extern vector<codeBlock> funs;  //I'm here!
extern bool in_block;

auto find_var(const string &s)
{
    auto it = vars.rbegin();
    for (; it != vars.rend(); it++)
    {
        if (it->name == s)
            break;
    }
    return it;
}

auto find_fun(const string &s)
{
    auto it = funs.begin();
    for (; it != funs.end(); it++)
    {
        if (it->name == s)
            break;
    }
    return it;
}

int codeBlock::exec(QPainter &painter)
{
    bool old_status = in_block;
    in_block = 1;
    int tot = this->type == 0 ? 1 : type;
    for (int i = 1; i <= tot; i++)
    {
        int ex_size = vars.size();
        /*for (int ii = 0; ii <= cmds.size() - 1; ii++)
        {
            myExe(cmds[ii], painter, );
        }*/
        for (auto it = cmds.begin(); it != cmds.end(); it++)
        {
            myExe(*it, painter, it);
        }
        //delete the local variates
        //auto start_pos = begin(vars) + ex_size;
        //auto end_pos = begin(vars) + vars.size();
        //vars.erase(start_pos, end_pos);
        vars.erase(vars.begin() + ex_size, vars.end());
    }
    in_block = old_status;
    return 0;
}

double s2num(const string &_s)
{
    int minus = 1;
    string s;
    if (_s[0] == '-')    //-1.345, l = 6
    {
        s = _s.substr(1, _s.length() - 1);
        minus = -1;
    }
    else
        s = _s;
    //deal with '-' completed
    auto point_pos = s.find('.');
    int l = s.length();
    if (point_pos == s.npos)    //integer type
    {
        if ('0' <= s[0] && s[0] <= '9')
        {
            int ten = 1;
            int num = 0;
            for (int i = l - 1; i >= 0; i--)
            {
                num += (s[i] - '0') * ten;
                ten *= 10;
            }
            return minus * num;
        }
        else if (('A' <= s[0] && s[0] <= 'Z') || ('a' <= s[0] && s[0] <= 'z'))
        {
            auto it = find_var(s);
            if (it != vars.rend())   //find it!
            {
                return minus * it->var;
            }
            else
                errorOccurred();
        }
        else
            errorOccurred();
    }
    else
    {
        double num = 0;
        num += s2num(s.substr(0, point_pos));
        double ten = 0.1;
        for (int i = point_pos + 1; i <= l - 1; i++)
        {
            num += (s[i] - '0') * ten;
            ten /= 10;
        }
        return minus * num;
    }
}

string num2s(const double &a)
{
    string s;
    stringstream ss;
    ss << a;
    s = ss.str();
    return s;
}

int ini_list()
{
    tokens["DEF"]   = { 1, 2 };
    tokens["ADD"]   = { 2, 2 };
    tokens["MOVE"]  = { 3, 1 };
    tokens["TURN"]  = { 4, 1 };
    tokens["COLOR"] = { 5, 3 };
    tokens["CLOAK"] = { 6, 0 };
    tokens["LOOP"]  = { 7, 1 };
    tokens["END"]   = { 8, 1 };
    tokens["CALL"]  = { 9, 1 };
    tokens["FUNC"]  = { 10, 1 };

    return 0;
}

//by Colin
//There are too many arguments! It seems ugly, but it CAN run whatever.
//So, let it be!
int readHead(int &width, int &height, int &r, int &g, int &b, double &xpos, double &ypos)
{
    string in;
    for (int i = 1; i <= 3; i++)
    {
        infile >> in;
        if (in == "@SIZE")
        {
            int w, h;
            infile >> w >> h;
            if (320 <= w && w <= 1920 && 240 <= h && h <= 1080)
            {
                width = w;
                height = h;
            }
            else
                errorOccurred();    //? head data imcomplete
        }
        else if (in == "@BACKGROUND")
        {
            int rr, gg, bb;
            infile >> rr >> gg >> bb;
            if (0 <= rr && rr <= 255 && 0 <= gg && gg <= 255 && 0 <= bb && bb <= 255)
            {
                r = rr;
                g = gg;
                b = bb;
            }
            else
                errorOccurred();
        }
        else if (in == "@POSITION")
        {
            int x, y;
            infile >> x >> y;
            if (0 <= x && x <= width - 1 && 0 <= y && y <= height - 1 )
            {
                xpos = x;
                ypos = y;
            }
            else
                errorOccurred();
        }
        else
            errorOccurred();    //? head data imcomplete
    }

    return 0;
}

//by Colin
int readLine(myCmd &cmd)  //read Cmd
{
    string input;
    while (infile >> input)
    {
        auto find = tokens.find(input);
        if (find != tokens.end())
        {
            cmd.token = find->second.id;
            if (cmd.token != 9 && cmd.token != 10)
            {
                for (int i = 1; i <= find->second.var_num; i++)
                {
                    infile >> input;
                    cmd.data.push_back(input);
                }
            }
            else    //read the whole line when reach FUNC or CALL (to avoid possible space)
            {
                getline(infile, input);
                cmd.data.push_back(input);
            }
            return cmd.token;
        }
        else
            return -1;
    }
    return -2;  //read EOF
}

//by Colin
int myExe(const myCmd &cmd, QPainter &painter, vector<myCmd>::iterator &in_block_cmd)
{
    if (cmd.token == 1) //DEF [Name] [value]
    {
        vars.push_back( { cmd.data[0], s2num(cmd.data[1]) } );
    }
    else if (cmd.token == 2)    //ADD [Name] [value]
    {
        auto it = find_var(cmd.data[0]);
        if (it != vars.rend())  //find it!
        {
            it->var += s2num(cmd.data[1]);
        }
        else
            errorOccurred();
    }
    else if (cmd.token == 3)    //MOVE [Step]
    {
        turtle.move(s2num(cmd.data[0]), painter);
    }
    else if (cmd.token == 4)    //TURN [Angle]
    {
        turtle.turn(s2num(cmd.data[0]));
    }
    else if (cmd.token == 5)    //COLOR [R] [G] [B]
    {
        turtle.appear();
        int r = s2num(cmd.data[0]);
        int g = s2num(cmd.data[1]);
        int b = s2num(cmd.data[2]);
        QPen pen;
        pen.setColor(QColor(r, g, b));
        pen.setWidth(3);
        pen.setStyle(Qt::SolidLine);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        painter.setPen(pen);
    }
    else if (cmd.token == 6)    //CLOAK
    {
        turtle.cloak();
    }
    else if (cmd.token == 7)    //LOOP [value]
    {
        //construct a codeBlock first
        const int loop_num = s2num(cmd.data[0]);
        codeBlock loop_block(loop_num);
        int fake_stack = 0;
        while (1)
        {
            myCmd sub_cmd;
            int ret = 0;
            if (in_block == 0)
                ret = readLine(sub_cmd);
            else    //in_block == 1
            {
                in_block_cmd++;
                ret = in_block_cmd->token;
                sub_cmd.token = ret;
                sub_cmd.data = in_block_cmd->data;
            }
            if (ret == 7)
                fake_stack++;
            if (ret > 0)
            {
                if (ret != 8)   //not reach END LOOP
                {
                    loop_block.cmds.push_back(sub_cmd);
                }
                else    //read END
                {
                    if (sub_cmd.data[0] == "LOOP")
                    {
                        if (fake_stack == 0)
                            break;  //stop while loop (stop inserting sub_cmd)
                        else
                        {
                            loop_block.cmds.push_back(sub_cmd);
                            fake_stack--;
                        }
                    }
                    else
                        errorOccurred();
                }
            }
            else
                errorOccurred();
            //end if
        }//end while
        //Execute!
        loop_block.exec(painter);
    }
    else if (cmd.token == 10)   //FUNC [Name]([Para 1],[Para 2],...,[Para n])
    {
        //const int fun_type = 0;
        codeBlock fun_block(0);
        fun_block.para_num = 0;
        auto cmd_data_length = cmd.data[0].length();
        int i_pos = 0;
        //store function name first
        fun_block.name = "";
        for (i_pos = 0; i_pos <= cmd_data_length - 1; i_pos++)
        {
            char tmp = (cmd.data[0])[i_pos];
            if (tmp != ' ') //ignore all spaces
            {
                if (tmp == '(')
                    break;
                else
                {
                    fun_block.name += tmp;
                }
            }
        }//end for
        while ((cmd.data[0])[++i_pos] == ' ')
            /* ignore possible spaces */;
        //store parameters
        string var_name("");
        //while ((cmd.data[0])[i_pos] != ')')  //( para1 , para2 )    ; ( )
        //while ((fun_block.cmds.size() == 0 && (cmd.data[0])[i_pos] != ')') || fun_block.cmds.size() != 0)
        while (var_name.length() == 0 && (cmd.data[0])[i_pos] != ')' || var_name.length() != 0)
        {
            if (i_pos >= cmd_data_length)
            {
                errorOccurred();
                break;
            }
            if ((cmd.data[0])[i_pos] != ' ')
            {
                char tmp = (cmd.data[0])[i_pos];
                if (tmp != ',' && tmp != ')')
                {
                    var_name += tmp;
                }
                else
                {
                    //add a new parameter definition(named var_name)!
                    myCmd para_def;
                    para_def.token = 1;
                    para_def.data.push_back(var_name);
                    para_def.data.push_back("0");
                    fun_block.cmds.push_back(para_def);
                    fun_block.para_num++;
                    var_name = "";
                    if (tmp == ')')
                        break;
                }
            }
            i_pos++;
        }//end while
        //store commands inside the function
        fun_block.cmd_num = 0;
        while (1)
        {
            myCmd in_fun_cmd;
            int ret = readLine(in_fun_cmd);
            if ((ret > 0 && ret != 8) || (ret == 8 && in_fun_cmd.data[0] != "FUNC"))
            {
                if (ret != 10)
                {
                    fun_block.cmds.push_back(in_fun_cmd);
                    fun_block.cmd_num++;
                }
                else
                    errorOccurred();    //You can't define a function inside a function!
            }
            else if (ret == -1) //read invalid token
                errorOccurred();
            else if (ret == -2 || ret == 8)
                break;
        }
        //store the function
        funs.push_back(fun_block);
    }//end else if of FUNC
    else if (cmd.token == 9)    //CALL [Name]([Para 1],[Para 2],...,[Para n])
    {
        auto cmd_data_length = cmd.data[0].length();
        int i_pos = 0;
        //store function name first
        string fun_name = "";
        for (i_pos = 0; i_pos <= cmd_data_length - 1; i_pos++)
        {
            char tmp = (cmd.data[0])[i_pos];
            if (tmp != ' ') //ignore all spaces
            {
                if (tmp == '(')
                    break;
                else
                {
                    fun_name += tmp;
                }
            }
        }//end for
        auto this_fun = find_fun(fun_name);
        //auto ex_size = this_fun->cmds.size();
        //auto origin_end = cmd_it + ex_size;
        if (this_fun != funs.end()) //find this function!
        {
            while ((cmd.data[0])[++i_pos] == ' ')
                /* ignore possible spaces */;
            //initialize the parameters (insert "ADD" commands)
            auto cmd_it = this_fun->cmds.begin();
            string para("");
            //while ((cmd.data[0])[i_pos] != ')')  //( para1 , para2 )    ; ( )
            int have_ini = 0;
            //while ((this_fun->cmds.size() == 0 && (cmd.data[0])[i_pos] != ')') || this_fun->cmds.size() != 0)
            while (para.length() == 0 && (cmd.data[0])[i_pos] != ')' || para.length() != 0)
            {
                if (i_pos >= cmd_data_length)
                {
                    errorOccurred();
                    break;
                }
                if ((cmd.data[0])[i_pos] != ' ')
                {
                    char tmp = (cmd.data[0])[i_pos];
                    if (tmp != ',' && tmp != ')')
                    {
                        para += tmp;
                    }
                    else
                    {
                        if (cmd_it != this_fun->cmds.begin() + this_fun->para_num)
                        {
                            //initialize this parameter(named var_name) for the function(this_fun)!
                            myCmd ini_var;
                            ini_var.token = 2;  //ADD
                            ini_var.data.push_back(cmd_it->data[0]);
                            ini_var.data.push_back(num2s(s2num(para)));
                            //this_fun->cmds.push_back(ini_var);
                            this_fun->cmds.insert(this_fun->cmds.end() - this_fun->cmd_num, ini_var);
                            have_ini++;
                            cmd_it = this_fun->cmds.begin() + have_ini;
                            para = "";
                            if (tmp == ')')
                                break;
                        }
                        else
                            errorOccurred();    //number of parameters exceed!
                    }
                }
                i_pos++;
            }//end while
            //initializing ended
            if (cmd_it != this_fun->cmds.end() - this_fun->cmd_num)
                /*Warning: number of parameters is less than the definition*/;
        }//end if
        else
            errorOccurred();    //Can't find function named fun_name
        //CALL the FUNC!
        this_fun->exec(painter);
        //delete the "ADD" command of parameters 123123456
        this_fun->cmds.erase(this_fun->cmds.begin() + this_fun->para_num, this_fun->cmds.end() - this_fun->cmd_num);
    }//end else if of CALL

    else
        errorOccurred();

    return 0;
}

void errorOccurred()
{
    cout << "Error occurred!" << endl;
}

#endif // MYFUN_H
