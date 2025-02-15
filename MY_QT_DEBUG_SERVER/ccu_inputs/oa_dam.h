#ifndef OA_DAM_H
#define OA_DAM_H

#include <QObject>
#include "shm_data_gaziray.h"
#include <QTableWidget>
#include <QDebug>
#include "utils.h"
#include <QPushButton>
#include <QByteArray>
#include <QVBoxLayout>




class Oa_Dam : public QObject
{
    Q_OBJECT
public:
    explicit Oa_Dam (QObject *parent = nullptr);
    enum OA_MVB_NUM {MVB1};
    static constexpr int OA_DAM_SIZE = sizeof(oa_dam);


    ~Oa_Dam();
    QTableWidget* getTableWidget(OA_VEHICLE_NUM  oa_type){
        //qDebug() << " OA_DAM_SIZE is : " << OA_DAM_SIZE;
        return m_tableWidget[static_cast<int>(oa_type)];
    }

    dam_oa& oa_x_dam(OA_VEHICLE_NUM oa_type) {
        return  m_oa_x_dam[static_cast<int>(oa_type)].oa_dam;
    }

    std::array<uint8_t, OA_DAM_SIZE> oa_x_dam_arr(OA_VEHICLE_NUM oa_type) const
    {
        std::array<uint8_t, OA_DAM_SIZE> tempArr;
        memcpy(tempArr.begin(), &m_oa_x_dam[static_cast<int>(oa_type)], OA_DAM_SIZE);
        return tempArr;
    }

    std::vector<uint8_t> moduleData(OA_VEHICLE_NUM oa_x_num) const
    {
        std::vector<uint8_t> tempData(OA_DAM_SIZE);
        std::memcpy(tempData.data(), &m_oa_x_dam[static_cast<int>(oa_x_num)], OA_DAM_SIZE);
        return tempData;
    }

    uint16_t portId(OA_VEHICLE_NUM oa_x_num) const
    {
        return m_port_id.at(static_cast<int>(oa_x_num));
    }

    void update_mvb1_map(OA_VEHICLE_NUM oa_x_num);
    void update_table(OA_VEHICLE_NUM oa_x_num);


private:
    void make_signal_slot_connection();
    int get_index(OA_VEHICLE_NUM num) const {
        return static_cast<int>(num);
    }
    int to_index(OA_VEHICLE_NUM num) const {
        return static_cast<int>(num);
    }
    void update_struct_with_map(OA_VEHICLE_NUM oa_x_num, OA_MVB_NUM mvb_num);
    void set_struct_mvb1(OA_VEHICLE_NUM oa_x_num);
    void init_oa_x_mvb1_table(OA_VEHICLE_NUM vehicle_type);


private:
    const QStringList table_name[2] = {
        {"oa_1_dam", "value"},
        {"oa_2_dam", "value"}
    };

    struct oa_dam m_oa_x_dam[2];
    std::map<QString, int> m_inputs_map[2];
    QTableWidget *m_tableWidget[2];
    std::vector<uint16_t> m_port_id {0xb62, 0xb82}; // Direct initialization

};

#endif //
