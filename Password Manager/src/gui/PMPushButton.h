#pragma once
#include <qpushbutton.h>
#include <QPropertyAnimation>
#include <QEvent>
#include <unordered_map>
#include "../resources/pm.h"
class PMPushButton : public QPushButton
{
	Q_OBJECT
		Q_PROPERTY(QColor backgroundColor READ GetColor WRITE SetColor)
		Q_PROPERTY(QColor borderColor READ GetBorderColor WRITE SetBorderColor)

public:
	PMPushButton(QWidget* parent = nullptr);

	bool hovering = false;
	QColor baseColor;
	QColor hoverColor;
	QColor pushColor;

	QColor baseBorderColor;
	QColor hoverBorderColor;
	QColor focusedBorderColor;

	QIcon baseIcon;
	QIcon hoverIcon;

	void SetColor(const QColor& color);
	const QColor& GetColor() const noexcept;

	void SetBorderColor(const QColor& color);
	const QColor& GetBorderColor() const noexcept;

	void StartFocusAnimation();
	void StopFocusAnimation();
	void StartHoverEnterAnimation();
	void StopHoverEnterAnimation();
	void StartMousepressAnimation();
	void StopMousepressAnimation();

	void AutoSetColorBasedOn(const QColor&);

	void updateStyleSheet();
	const QString GetStyleSheet();

	void SetQssProperty(QString prop, QString value);
	const QString GetQssProperty(QString prop);

	void SetIconOnly(QIcon base, QIcon hover);
	void SetFontColor(const QColor& color);
protected:
	bool eventFilter(QObject* obj, QEvent* e) override;

private:
	boolean isIconButton = false;
	QColor backgroundColor;
	QColor borderColor;
	std::unordered_map<QString, QString> qss;
};