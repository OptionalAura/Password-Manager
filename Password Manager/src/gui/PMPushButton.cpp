#pragma once
#include "PMPushButton.h"

PMPushButton::PMPushButton(QWidget* parent) :
	QPushButton(parent)
{
	this->installEventFilter(this);
	SetQssProperty("border-radius", QString("3px"));
	SetQssProperty("padding", "4px 8px");
	//SetQssProperty("color", "rgba(255, 255, 255, 0.65)");
	SetQssProperty("font", "10pt \"Segoe UI\"");
	this->AutoSetColorBasedOn(QColor("#333333"));
	//this->SetFontColor(pm::getOptimalFontColor());
	LOG(GetStyleSheet().toStdString());
}
void PMPushButton::SetIconOnly(QIcon hover, QIcon base) {
	isIconButton = true;
	this->baseIcon = base;
	this->hoverIcon = hover;
	this->setIcon(base);
	this->setText("");
	this->SetQssProperty("padding", "0px");
	this->AutoSetColorBasedOn(QColor(0, 0, 0, 0));
}

void PMPushButton::SetColor(const QColor& color)
{
	this->backgroundColor = color;
	QString qss;
	SetQssProperty("background-color", QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
	updateStyleSheet();
}

void PMPushButton::updateStyleSheet() {
	setStyleSheet(GetStyleSheet());
}

const QString PMPushButton::GetStyleSheet() {
	QString format("QPushButton{");
	for (std::pair<QString, QString> pair : qss) {
		format.append(pair.first).append(":").append(pair.second).append(";");
	}
	format.append("}");
	return format;
}

void PMPushButton::SetQssProperty(QString prop, QString value) {
	qss.insert_or_assign(prop, value);
}

const QString PMPushButton::GetQssProperty(QString prop) {
	return qss.at(prop);
}

void PMPushButton::SetFontColor(const QColor& color)
{
	SetQssProperty("color", QString("rgba(%1, %2, %3, %4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}

const QColor& PMPushButton::GetColor() const noexcept
{
	return backgroundColor;
}
void PMPushButton::SetBorderColor(const QColor& color)
{
	this->borderColor = color;
	QString qss;
	SetQssProperty("border", QString("1px solid rgb(%1, %2, %3)").arg(color.red()).arg(color.green()).arg(color.blue()));
	updateStyleSheet();
}
const QColor& PMPushButton::GetBorderColor() const noexcept {
	return this->borderColor;
}
void PMPushButton::AutoSetColorBasedOn(const QColor& color) {
	this->SetColor(color);
	this->baseColor = color;
	this->hoverColor = color.darker(140);
	this->pushColor = color.darker(200);
	this->SetFontColor(pm::getOptimalFontColor(color));
	updateStyleSheet();
}

bool PMPushButton::eventFilter(QObject* obj, QEvent* e)
{
	switch (e->type()) {
	case QEvent::HoverEnter:
		StartHoverEnterAnimation();
		if (isIconButton)
			this->setIcon(hoverIcon);
		LOG("Hover enter");
		break;
	case QEvent::HoverLeave:
		StopHoverEnterAnimation();
		if (isIconButton)
			this->setIcon(baseIcon);
		LOG("Hover leave");
		break;
	case QEvent::MouseButtonPress:
		StartMousepressAnimation();
		LOG("Mouse down");
		break;
	case QEvent::MouseButtonRelease:
		StopMousepressAnimation();
		LOG("Mouse up");
		break;
	}
	return false;
}

void PMPushButton::StartFocusAnimation() {
	auto animation = new QPropertyAnimation(this, "borderColor");
	animation->setDuration(200);
	animation->setStartValue(hovering ? hoverBorderColor : baseBorderColor);
	animation->setEndValue(focusedBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}
void PMPushButton::StopFocusAnimation() {
	auto animation = new QPropertyAnimation(this, "borderColor");
	animation->setDuration(200);
	animation->setStartValue(focusedBorderColor);
	animation->setEndValue(hovering ? hoverBorderColor : baseBorderColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PMPushButton::StartMousepressAnimation() {
	auto animation = new QPropertyAnimation(this, "backgroundColor");
	animation->setDuration(50);
	animation->setStartValue(hoverColor);
	animation->setEndValue(pushColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PMPushButton::StopMousepressAnimation()
{
	auto animation = new QPropertyAnimation(this, "backgroundColor");
	animation->setDuration(200);
	animation->setStartValue(pushColor);
	animation->setEndValue(hoverColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PMPushButton::StartHoverEnterAnimation()
{
	auto animation = new QPropertyAnimation(this, "backgroundColor");
	animation->setDuration(200);
	animation->setStartValue(baseColor);
	animation->setEndValue(hoverColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}

void PMPushButton::StopHoverEnterAnimation()
{
	auto animation = new QPropertyAnimation(this, "backgroundColor");
	animation->setDuration(200);
	animation->setStartValue(hoverColor);
	animation->setEndValue(baseColor);
	animation->start(QAbstractAnimation::DeleteWhenStopped);
}