#include <QApplication>
// login.cpp 或类似文件
#include "login/LoginWindow.h"
#include "chat/ChatWindow.h"
int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	LoginWindow login;
	if(login.exec() == QDialog::Accepted)
	{
		ChatWindow w(login.getUserID());
		w.show();
		app.exec();
	}
	return 0;
}