#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <unordered_set>

enum OrderType { BUY, SELL };

struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;
};

class OrderBook {
private:
    std::map<double, std::deque<Order>> asks;
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    std::unordered_set<int> cancelled_orders;

public:
    void add_order(int id, OrderType type, double price, int quantity) {
        Order new_order = {id, type, price, quantity};

        if (type == BUY) {
            match_buy(new_order);
        } else {
            match_sell(new_order);
        }
    }

    void cancel_order(int id) {
        cancelled_orders.insert(id);
        std::cout << "[CANCEL] Order #" << id << " has been cancelled by user.\n";
    }

    void match_buy(Order& order) {
        while (order.quantity > 0 && !asks.empty()) {
            auto best_ask_it = asks.begin();
            double seller_price = best_ask_it->first;
            std::deque<Order>& orders_at_price = best_ask_it->second;

            Order& sell_order = orders_at_price.front();

            if (cancelled_orders.count(sell_order.id)) {
                std::cout << "[SKIP] Found cancelled SELL Order #" << sell_order.id << ". Removing from book.\n";
                orders_at_price.pop_front();
                if (orders_at_price.empty()) {
                    asks.erase(best_ask_it);
                }
                continue;
            }

            if (seller_price > order.price) {
                break; 
            }

            int trade_qty = std::min(order.quantity, sell_order.quantity);

            std::cout << "[TRADE] BUY #" << order.id << " matched with SELL #" << sell_order.id 
                      << " -> " << trade_qty << " units for $" << seller_price << std::endl;

            order.quantity -= trade_qty;
            sell_order.quantity -= trade_qty;

            if (sell_order.quantity == 0) {
                orders_at_price.pop_front();
                if (orders_at_price.empty()) {
                    asks.erase(best_ask_it);
                }
            }
        }

        if (order.quantity > 0) {
            bids[order.price].push_back(order);
            std::cout << "[BOOK] BUY #" << order.id << " added to order book (" << order.quantity << " remaining)." << std::endl;
        }
    }

    void match_sell(Order& order) {
        while (order.quantity > 0 && !bids.empty()) {
            auto best_bid_it = bids.begin();
            double buyer_price = best_bid_it->first;
            std::deque<Order>& orders_at_price = best_bid_it->second;

            Order& buy_order = orders_at_price.front();

            if (cancelled_orders.count(buy_order.id)) {
                std::cout << "[SKIP] Found cancelled BUY Order #" << buy_order.id << ". Removing from book.\n";
                orders_at_price.pop_front();
                if (orders_at_price.empty()) {
                    bids.erase(best_bid_it);
                }
                continue;
            }

            if (buyer_price < order.price) {
                break;
            }

            int trade_qty = std::min(order.quantity, buy_order.quantity);

            std::cout << "[TRADE] SELL #" << order.id << " matched with BUY #" << buy_order.id 
                      << " -> " << trade_qty << " units for $" << buyer_price << std::endl;

            order.quantity -= trade_qty;
            buy_order.quantity -= trade_qty;

            if (buy_order.quantity == 0) {
                orders_at_price.pop_front();
                if (orders_at_price.empty()) {
                    bids.erase(best_bid_it);
                }
            }
        }

        if (order.quantity > 0) {
            asks[order.price].push_back(order);
            std::cout << "[BOOK] SELL #" << order.id << " added to order book (" << order.quantity << " remaining)." << std::endl;
        }
    }

    void print_status() {
        std::cout << "\n=== ORDER BOOK ===\n";
        std::cout << "--- Sellers ---\n";
        for (auto it = asks.rbegin(); it != asks.rend(); ++it) {
            int active_qty = 0;
            for(const auto& o : it->second) {
                if (!cancelled_orders.count(o.id)) active_qty += o.quantity;
            }
            if (active_qty > 0)
                std::cout << "Price: $" << it->first << " | Orders Qty: " << active_qty << "\n";
        }
        std::cout << "--- Buyers ---\n";
        for (auto& pair : bids) {
            int active_qty = 0;
            for(const auto& o : pair.second) {
                if (!cancelled_orders.count(o.id)) active_qty += o.quantity;
            }
            if (active_qty > 0)
                std::cout << "Price: $" << pair.first << " | Orders Qty: " << active_qty << "\n";
        }
        std::cout << "==================================\n\n";
    }
};

int main() {
    OrderBook engine;

    
    
    engine.add_order(1, BUY, 100.0, 10);
    engine.add_order(2, BUY, 99.0, 20);
    engine.add_order(3, BUY, 98.0, 50);
    engine.add_order(4, SELL, 101.0, 10);
    engine.add_order(5, SELL, 102.0, 20);
    
    engine.print_status();

    engine.add_order(6, SELL, 98.0, 60);

    engine.print_status();

    engine.add_order(7, SELL, 105.0, 100);
    engine.add_order(8, SELL, 105.0, 100);
    
   
    engine.add_order(9, BUY, 105.0, 100);

    engine.print_status();

   
    engine.add_order(10, BUY, 90.0, 10000);
    engine.print_status();
    
    engine.cancel_order(10);
    
    engine.add_order(11, SELL, 90.0, 5);

    engine.print_status();

    return 0;
}