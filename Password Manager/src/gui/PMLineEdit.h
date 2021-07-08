#pragma once
#include <QEvent>
#include <qlineedit.h>
#include <QPropertyAnimation>
#include <unordered_map>

#include "../PMGlobalDefs.h"

class PMLineEdit : public QLineEdit
{
	Q_OBJECT
		Q_PROPERTY(QColor backgroundcolor READ GetColor WRITE SetColor);
	Q_PROPERTY(QColor bordercolor READ GetBorderColor WRITE SetBorderColor);
public:
	PMLineEdit(QWidget* parent = nullptr);
	bool hovering = false;
	QColor baseColor;
	QColor hoverColor;
	QColor pushColor;

	QColor baseBorderColor;
	QColor hoverBorderColor;
	QColor focusedBorderColor;

	void SetColor(const QColor&);
	const QColor& GetColor() const noexcept;

	void SetBorderColor(const QColor&);
	const QColor& GetBorderColor() const noexcept;

	void StartFocusAnimation();
	void StopFocusAnimation();
	void StartHoverEnterAnimation();
	void StopHoverEnterAnimation();
	void StartMousepressAnimation();
	void StopMousepressAnimation();

	void AutoSetColorBasedOn(const QColor&);
	void AutoSetBorderColorBasedOn(const QColor&);

	void updateStyleSheet();
	const QString GetStyleSheet();

	void SetQssProperty(QString, QString);
	const QString GetQssProperty(QString) const;

	void SetFontColor(const QColor&);
protected:
	bool eventFilter(QObject*, QEvent*) override;
private:
	QColor backgroundcolor;
	QColor bordercolor;
	std::unordered_map<QString, QString> qss;
};
