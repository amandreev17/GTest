#include "Account.h"
#include "Transaction.h"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class AccountMock : public Account {
public:
	AccountMock(int id, int balance) : Account(id, balance) {}
	MOCK_CONST_METHOD0(GetBalance, int());
	MOCK_METHOD1(ChangeBalance, void(int diff));
	MOCK_METHOD0(Lock, void());
	MOCK_METHOD0(Unlock, void());
};
class TransactionMock : public Transaction {
public:
	MOCK_METHOD3(Make, bool(Account& from, Account& to, int sum));
};

TEST(Account, Mock) {
	AccountMock account(1, 100);
	EXPECT_CALL(account, GetBalance()).Times(1);
	EXPECT_CALL(account, ChangeBalance(testing::_)).Times(1);
	EXPECT_CALL(account, Lock()).Times(1);
	EXPECT_CALL(account, Unlock()).Times(1);
	account.GetBalance();
	account.ChangeBalance(20);
	account.Lock();
	account.Unlock();
}

TEST(Account, Init) {
    AccountMock account(1, 1000);
    EXPECT_EQ(account.id(), 1);
    EXPECT_EQ(1000, account.Account::GetBalance());
}

TEST(Account, GetBalance) {
    AccountMock account(1, 1000);
    EXPECT_EQ(1000, account.Account::GetBalance());
}

TEST(Account, ChangeBalance) {
    AccountMock account(1, 1000);
    account.Account::Lock();
    account.Account::ChangeBalance(100);
    EXPECT_NO_THROW(account.Account::Unlock());
    EXPECT_EQ(1100, account.Account::GetBalance());
}

TEST(Account, Lock) {
    AccountMock account(1, 100);
    EXPECT_THROW(account.Account::ChangeBalance(100), std::runtime_error);
}

TEST(Account, Unlock) {
    AccountMock account(1, 100);
    EXPECT_NO_THROW(account.Account::Unlock());
}

TEST(Transaction, Mock) {
	TransactionMock tr;
	Account ac1(1, 50);
	Account ac2(2, 500);
	EXPECT_CALL(tr, Make(testing::_, testing::_, testing::_)).Times(1);
	tr.Make(ac1, ac2, 199);
}

TEST(Transaction, Make) {
    Transaction tr;
    Account account1(1, 200);
    Account account2(1, 200);
    Account account3(2, 200);
    EXPECT_THROW(tr.Make(account1, account2, 150), std::logic_error);
    EXPECT_THROW(tr.Make(account1, account3, -10), std::invalid_argument);
    EXPECT_THROW(tr.Make(account1, account2, 99), std::logic_error);
    EXPECT_EQ(tr.fee() * 2 > 150, false);
}

TEST(Transaction, SaveToDataBase) {
    Transaction tr;
    Account account1(1, 2000);
    Account account2(2, 200);
    tr.set_fee(0);
    tr.Make(account1, account2, 400);
    EXPECT_EQ(account1.GetBalance(), 1600);
    EXPECT_EQ(account2.GetBalance(), 600);
}
