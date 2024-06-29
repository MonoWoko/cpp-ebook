#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <map>
#include <string>
#include <vector>

// Устанавливаем точность вывода в программе
int const CURRENT_OUT_PRECISION = 6;

class CheerManager {
public:
    //Функция вносит прогресс по конкретному пользовотелю и обнавляет общую статистику
    void AddUserProgres(int user_id, int page) {
        if (users_read_progres_.find(user_id) == users_read_progres_.end()) {
            users_read_progres_.emplace(std::move(user_id), page);
            UpdateOverallUsersProgres(0, std::move(page));
            return;
        }
        UpdateOverallUsersProgres(users_read_progres_.at(user_id), page);
        users_read_progres_.at(user_id) = std::move(page);
        return;
    }

    //Функция для подбадривания конкретного пользователя
    void CheerUser(int user_id, std::ostream& output) const {
        // Проверка, есть ли пользователь в базе:
        if ((users_read_progres_.size() == 0) || (users_read_progres_.find(user_id) == users_read_progres_.end())) {
            output << 0 << '\n';
            return;
        }
        // Проверка, случая если пользователь один:
        if (users_read_progres_.size() == 1) {
            output << 1 << '\n';
            return;
        }
        // Вычисление доли пользователей (без учета запрашиваемого) с прогрессом ниже текущего пользователя:
        // 1. Исключаем запрашиваемого пользователя из статистики
        int users_count_without_current_user = users_read_progres_.size() - 1;
        // 2. производим основные статистические вычисления (преобразуем числитель в double умнажением ни 1.0) и выводим результат.
        std::cout << (users_read_progres_.size() - users_on_each_page_count_.at(users_read_progres_.at(user_id))) * 1.0 /* Количество людей с прогрессом ниже*/
                                                        / 
                                    users_count_without_current_user << '\n'; /* Количество пользователей (без учета запрашиваемого)*/
        return;
    }

private:
    //Функция обнавляет общую статистику прогресса пользователей 
    void UpdateOverallUsersProgres(int previous_page, int current_page) {
        while (users_on_each_page_count_.size() <= current_page) {
            users_on_each_page_count_.push_back(0);
        }
        while (previous_page < current_page) {
            ++previous_page;
            ++users_on_each_page_count_.at(previous_page);
        }
        return;
    }

    std::map<int, int> users_read_progres_;
    std::vector<int> users_on_each_page_count_;
};

void OperateRequests(int request_count, CheerManager& cheer_manager, std::istream& input, std::ostream& output) {
    output << std::setprecision(CURRENT_OUT_PRECISION);
    while (request_count > 0) {
            std::string command;
            input >> command;
            if (command == "READ") {
                int user_id, page;
                input >> user_id >> page;
                cheer_manager.AddUserProgres(user_id, page);
            } else {
                int user_id;
                input >> user_id;
                cheer_manager.CheerUser(user_id, output);
            }
            --request_count;
    }
    return;
}

int main()
{
    CheerManager cheer_manager;
    int request_count;
    std::cin >> request_count;

    OperateRequests(std::move(request_count), cheer_manager, std::cin, std::cout);
    return 0;
}
