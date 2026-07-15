#include "login.h"
#include "ui_login.h"
#include<QToolButton>
#include<QPixmap>
#include "widget.h"
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    QVector<QToolButton*> vector;   //存放按钮的容器
    ui->setupUi(this);  //显示ui布局
    this->setWindowIcon(QIcon(":/images/qq.png"));
    this->setWindowTitle("QQ");
    //用户名列表
    QStringList list;
    list<<"张三"<<"李四"<<"王五"<<"xrg"<<"cdq"<<"dyt"<<"mgj"<<"lxc"<<"csm";
    //图片资源列表
    QStringList listIcon;
    listIcon<<"spqy"<<"ymrl"<<"qq"<<"xrg"<<"cdq"<<"dyt"<<"mgj"<<"lxc"<<"spqy";

    //循环创建按钮对象及其对应姓名
    for(int i=0;i<9;i++){
        QToolButton *btn=new QToolButton(this);	//父对象为this 保证了资源释放
        //设置按钮图标
        btn->setIcon(QPixmap(QString(":/images/%1.png").arg(listIcon[i])));
        //设置图标大小为对应图片的大小
        btn->setIconSize(QPixmap(QString(":/images/%1.png").arg(listIcon[i])).size());
        //设置按钮为透明效果（自动凸起）
        btn->setAutoRaise(true);
        //设置网名
        btn->setText(QString("%1").arg(list[i]));
        //设置按钮的显示格式，使文本显示在图标旁边 貌似只有这一种显示方式。。。
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //将按钮添加到垂直布局管理器ui->vlayout中
        ui->vlayout->addWidget(btn);
        vector.push_back(btn);   //每创建一个btn就加入到vector中
        isShow.push_back(false);
    }

    for(int i=0;i<9;i++){
        connect(vector[i],&QToolButton::clicked, this, [=](){
            if(true==isShow[i]){
                QMessageBox::warning(this,"警告","该聊天框已被打开");
                return;
            }
            isShow[i]=true;
            //打开对应用户的聊天窗口
            Widget *widget=new Widget(0,vector[i]->text());
            widget->setWindowIcon(vector[i]->icon());
            widget->setWindowTitle(vector[i]->text());
            widget->show();
            //关闭聊天框时将对于的isShow跟改为false
            connect(widget,&Widget::closeWidget,[=](){
                isShow[i]=false;
            });
         });
    }
}

Login::~Login()
{
    delete ui;
}
