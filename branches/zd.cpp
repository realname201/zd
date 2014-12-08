#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

struct record
{
    string str_trans_date;          // 交易日
    string str_post_date;           // 记账日
    string str_description;         // 交易摘要
    string str_rmb_amount;          // 人民币金额
    int    i_card_number;           // 卡号末四位
    string str_country;             // 交易地点
    float  f_ori_trans_amount;      // 交易地金额
};

bool read(ifstream &if_read, record &st_record);
void print(ostream &of_write, record &st_record);
bool compare_card(const record &st_record1, const record &st_record2)
{
    return st_record1.i_card_number < st_record2.i_card_number;
}

bool compare_money(const record &st_record1, const record &st_record2)
{
    float f_1 = st_record1.f_ori_trans_amount;
    float f_2 = st_record2.f_ori_trans_amount;

    if (f_1 < 0)
    {
        f_1 = 0 - f_1;
    }
    if (f_2 < 0)
    {
        f_2 = 0 - f_2;
    }

    return f_1 < f_2;
}

int main(int argc, char *argv[])
{
    if (2 != argc)
    {
        return -1;
    }

    vector<record> vt_records;
    ifstream if_read(argv[1]);
    record st_record;

    while (true == read(if_read, st_record))
    {
        vt_records.push_back(st_record);
    }

    float f_total = 0.00;
    float f_card_total = 0.00;
    int i_card = 0;
    stable_sort(vt_records.begin(), vt_records.end(), compare_card);
    for (vector<record>::iterator it_idx = vt_records.begin(); it_idx != vt_records.end(); ++it_idx)
    {
        if (0 != i_card && i_card != it_idx->i_card_number)
        {
            printf("[%d, %.2f]\n\n", i_card, f_card_total);
            i_card = it_idx->i_card_number;
            f_card_total = it_idx->f_ori_trans_amount;
        }
        else
        {
            f_card_total += it_idx->f_ori_trans_amount;
            i_card = it_idx->i_card_number;
        }
        f_total += it_idx->f_ori_trans_amount;
        print(cout, *it_idx);
    }

    printf("[%d, %.2f]\n", i_card, f_card_total);
    printf("totals: %.2f\n", f_total);

    stable_sort(vt_records.begin(), vt_records.end(), compare_money);
    for (vector<record>::iterator it_idx = vt_records.begin(); it_idx != vt_records.end(); ++it_idx)
    {
        if (it_idx->f_ori_trans_amount >= 10.00 || it_idx->f_ori_trans_amount <= -10.00)
        {
            print(cout, *it_idx);
        }
    }
    return 0;
}

#define READ_STRING(io_input, str_value)    \
{\
    if (io_input)   \
    {   \
        getline(io_input, str_value); \
        str_value[str_value.length() - 1] = '\0';   \
     }   \
    else    \
    {   \
        return false;   \
    }   \
}

bool read(ifstream &if_read, record &st_record)
{
    string str_tmp;

    READ_STRING(if_read, st_record.str_trans_date);
    READ_STRING(if_read, st_record.str_post_date);
    READ_STRING(if_read, st_record.str_description);
    READ_STRING(if_read, st_record.str_rmb_amount);

    READ_STRING(if_read, str_tmp);
    st_record.i_card_number = 0;
    sscanf(str_tmp.c_str(), "%d", &st_record.i_card_number);

    READ_STRING(if_read, st_record.str_country);

    READ_STRING(if_read, str_tmp);

    st_record.f_ori_trans_amount = 0.00;
    sscanf(str_tmp.c_str(), "%f", &st_record.f_ori_trans_amount);

    return true;
}

void print(ostream &of_write, record &st_record)
{
    char sz_recode[512] = {0};

    sprintf(sz_recode, "%-4s  %-4s  %12s  %4d %4s %8.2f %s", st_record.str_trans_date.c_str(),
            st_record.str_post_date.c_str(), st_record.str_rmb_amount.c_str(),
            st_record.i_card_number, st_record.str_country.c_str(),
            st_record.f_ori_trans_amount, st_record.str_description.c_str());

    of_write << sz_recode << endl;
}
