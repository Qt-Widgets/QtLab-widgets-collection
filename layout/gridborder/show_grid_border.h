#ifndef GRIDTEST_H
#define GRIDTEST_H

#include <QGridLayout>
#include <QLabel>
#include <QDialog>

class GridTest : public QDialog
{
	Q_OBJECT
public:
	public:
		GridTest( QWidget* parent = 0);
		virtual ~GridTest();
	private:
		QList<QLabel*> m_labels;
	protected:
		QGridLayout* grid;
		QGridLayout* boardLayout;
};
#endif // GRIDTEST_H
