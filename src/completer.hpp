#ifndef CPDMENU_COMPLETER_HPP
#define CPDMENU_COMPLETER_HPP

#include <QListWidget>

class Completer : public QListWidget {
  Q_OBJECT

public:
  Completer(const QStringList &, QWidget *parent = 0);

  void trigger(const QString &text);
  void move(const int movement);

  QString currentText() const;

private:
  QStringList m_list;
};

#endif
