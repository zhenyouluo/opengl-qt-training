#ifndef ITEMACTION_H
#define ITEMACTION_H
#include <QAction>

class ItemAction :public QAction
{
	Q_OBJECT
public:
	ItemAction(const QString &text, QObject* parent);

signals:
	void itemAdd();

};

#endif