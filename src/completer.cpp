#include "completer.hpp"

Completer::Completer(const QStringList &list, QWidget *parent)
  : QListWidget(parent), m_list(list)
{
  setAttribute(Qt::WA_MacShowFocusRect, false);
  setFlow(QListView::LeftToRight);
  setFocusPolicy(Qt::NoFocus);
  setFrameShape(QFrame::NoFrame);
  setSortingEnabled(true);
  setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

  trigger({});
}

void Completer::trigger(const QString &input)
{
  QSignalBlocker blocker(this);
  clear();

  for(const QString &word : m_list) {
    if(word.contains(input))
      addItem(word);
  }

  setCurrentRow(0);
}

QString Completer::currentText() const
{
  if(const QListWidgetItem *item = currentItem())
    return item->text();
  else
    return {};
}

void Completer::move(const int movement)
{
  const int size = count();
  const int row = (currentRow() + movement) % size;

  setCurrentRow(row > -1 ? row : size - 1);
}
