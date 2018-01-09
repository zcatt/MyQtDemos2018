#ifndef ADDINTERFACE_H
#define ADDINTERFACE_H

class CAddInterface
{
public:
    virtual int add(int a, int b) = 0;
};

#define AddInterface_iid "com.github.zcatt.MyQtDemos2018.AddPlugin"

Q_DECLARE_INTERFACE(CAddInterface, AddInterface_iid)


#endif // ADDINTERFACE_H
