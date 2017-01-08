#include "prompt.hpp"

#include "completer.hpp"

#include <QKeyEvent>

Prompt::Prompt(Completer *completer, QWidget *parent)
  : QLineEdit(parent), m_promptSize(0), m_completer(completer)
{
  setFrame(false);
  setAttribute(Qt::WA_MacShowFocusRect, false);

  connect(this, &QLineEdit::textChanged, this, &Prompt::check);
  connect(this, &QLineEdit::editingFinished, this, &Prompt::send);
  connect(this, &QLineEdit::cursorPositionChanged, this, &Prompt::cursorMoved);
  connect(this, &QLineEdit::selectionChanged, this, &Prompt::selectionChanged);

  connect(m_completer, &Completer::currentItemChanged, this, &Prompt::replaceWord);
}

void Prompt::setPrompt(const QString &prompt)
{
  m_promptSize = prompt.size();
  QLineEdit::setText(prompt);
}

QString Prompt::prompt() const
{
  return QLineEdit::text().left(m_promptSize);
}

void Prompt::setText(const QString &userText)
{
  QLineEdit::setText(prompt() + userText);
}

QString Prompt::text() const
{
  return QLineEdit::text().remove(0, m_promptSize);
}

void Prompt::check()
{
  if(prompt().size() < m_promptSize)
    Q_EMIT editingFinished({});
  else
    m_completer->trigger(text());
}

void Prompt::send()
{
  if(hasFocus())
    Q_EMIT editingFinished(m_completer->currentText());
}

void Prompt::cursorMoved(const int, const int after)
{
  if(after < m_promptSize)
    setCursorPosition(m_promptSize);
}

void Prompt::selectionChanged()
{
  if(!hasSelectedText() || selectionStart() >= m_promptSize)
    return;

  if(cursorPosition() < m_promptSize)
    cursorForward(true);
  else
    setSelection(m_promptSize, text().size());
}

void Prompt::replaceWord()
{
  const QString &newText = m_completer->currentText();

  QSignalBlocker blocker(this);
  setText(newText);
  setCursorPosition(m_promptSize + newText.size());
}

void Prompt::keyPressEvent(QKeyEvent *event)
{
  event->accept();

  switch(event->key()) {
  case Qt::Key_Escape:
    Q_EMIT editingFinished({});
    break;
  case Qt::Key_Tab:
  case Qt::Key_Right:
  case Qt::Key_Down:
    m_completer->move(1);
    break;
  case Qt::Key_Backtab:
  case Qt::Key_Left:
  case Qt::Key_Up:
    m_completer->move(-1);
    break;
  default:
    QLineEdit::keyPressEvent(event);
  }
}
