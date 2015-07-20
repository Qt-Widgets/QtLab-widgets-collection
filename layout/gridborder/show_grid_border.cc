#include <QString>
#include "show_grid_border.h"

GridTest::GridTest(QWidget* parent) : QDialog(parent)
{
	boardLayout = new QGridLayout(this);
	//create the GUI
	grid = new QGridLayout(this); // 3 px margin and spacing
	//create the labels
	int n = 3;
	int m = 3;
	for (int i=0; i<n; i+=2)
	{
		for (int j=0; j<m; j++)
		{
			QLabel* label = new QLabel("label" + QString::number(i) + QString::number(j), this);
			grid->addWidget( label, i, j );
			m_labels.append(label);
		}
	}
        grid->addWidget(new QLabel("haha", this), 1, 1);
	grid->addWidget(new QLabel("hoho", this), 0, 5);
	grid->addWidget(new QLabel("hehe", this), 1, 4);
	boardLayout->addLayout( grid, 0, 0);

QPalette Pal(palette());
 
// set black background
Pal.setColor(QPalette::Background, Qt::darkGray);
setAutoFillBackground(true);
setPalette(Pal);
}

GridTest::~GridTest()
{
	// Qt deletes the labels.
}
