#include <QApplication>
#include <QCommandLineParser>
#include <QDesktopWidget>
#include <QHBoxLayout>
#include <QTextStream>

#include "prompt.hpp"
#include "completer.hpp"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  const QCommandLineOption promptOpt({"p", "prompt"},
    "Change the prompt label.", "TEXT", ">");

  QCommandLineParser parser;
  parser.addHelpOption();
  parser.addOption(promptOpt);
  parser.process(app);

  const QString &promptText = parser.value(promptOpt);

  QStringList list;
  QTextStream input(stdin);
  while(!input.atEnd()) {
    const QString &line = input.readLine();
    if(!line.isEmpty())
      list << line;
  }

  if(list.isEmpty())
    return -1;

  QWidget window;
  window.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
  window.setFixedSize(QApplication::desktop()->screenGeometry().width(), 22);

  Completer completer(list);
  Prompt prompt(&completer);

  if(!promptText.isEmpty())
    prompt.setPrompt(promptText + ' ');

  QHBoxLayout layout;
  layout.setSpacing(5);
  layout.setContentsMargins(5, 0, 2, 0);
  layout.addWidget(&prompt);
  layout.addWidget(&completer);
  layout.setStretch(0, 2);
  layout.setStretch(1, 15);

  window.setLayout(&layout);
  window.show();

  QObject::connect(&prompt, &Prompt::editingFinished, [&](const QString &value) {
    if(!value.isEmpty())
      QTextStream(stdout) << value << '\n';

    window.close();
  });

  return app.exec();
}
