#ifndef CPDMENU_PROMPT_HPP
#define CPDMENU_PROMPT_HPP

#include <QLineEdit>

class Completer;

class Prompt : public QLineEdit
{
  Q_OBJECT

public:
  Prompt(Completer *, QWidget *parent = 0);

  void setPrompt(const QString &);
  QString prompt() const;

  void setText(const QString &);
  QString text() const;

Q_SIGNALS:
  void editingFinished(const QString &);

protected:
  void keyPressEvent(QKeyEvent *) override;

private:
  void check();
  void send();
  void cursorMoved(const int, const int);
  void selectionChanged();
  void replaceWord(const QString &);

  int m_promptSize;
  Completer *m_completer;
};

#endif
