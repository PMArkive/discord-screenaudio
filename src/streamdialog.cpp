#include "streamdialog.h"
#include "virtmic.h"

#include <QComboBox>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QSizePolicy>
#include <QVBoxLayout>

StreamDialog::StreamDialog() : QWidget() {
  setAttribute(Qt::WA_QuitOnClose, false);

  {
    auto layout = new QVBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetFixedSize);

    m_videoGroupBox = new QGroupBox(this);
    m_videoGroupBox->setTitle("Video");
    m_videoGroupBox->setCheckable(true);
    layout->addWidget(m_videoGroupBox);

    {
      auto videoLayout = new QVBoxLayout(this);
      m_videoGroupBox->setLayout(videoLayout);

      auto resolutionLabel = new QLabel(this);
      resolutionLabel->setText("Resolution");
      videoLayout->addWidget(resolutionLabel);

      m_resolutionComboBox = new QComboBox(this);
      m_resolutionComboBox->addItem("2160p", "3840x2160");
      m_resolutionComboBox->addItem("1440p", "2560x1440");
      m_resolutionComboBox->addItem("1080p", "1920x1080");
      m_resolutionComboBox->addItem("720p", "1280x720");
      m_resolutionComboBox->addItem("480p", "854x480");
      m_resolutionComboBox->addItem("360p", "640x360");
      m_resolutionComboBox->addItem("240p", "426x240");
      m_resolutionComboBox->setCurrentText("720p");
      videoLayout->addWidget(m_resolutionComboBox);

      auto framerateLabel = new QLabel(this);
      framerateLabel->setText("Framerate");
      videoLayout->addWidget(framerateLabel);

      m_framerateComboBox = new QComboBox(this);
      m_framerateComboBox->addItem("144 FPS", 144);
      m_framerateComboBox->addItem("60 FPS", 60);
      m_framerateComboBox->addItem("30 FPS", 30);
      m_framerateComboBox->addItem("15 FPS", 15);
      m_framerateComboBox->addItem("5 FPS", 5);
      m_framerateComboBox->setCurrentText("30 FPS");
      videoLayout->addWidget(m_framerateComboBox);
    }

    m_audioGroupBox = new QGroupBox(this);
    m_audioGroupBox->setCheckable(true);
    m_audioGroupBox->setTitle("Audio");
    layout->addWidget(m_audioGroupBox);

    {
      auto audioLayout = new QVBoxLayout(this);
      m_audioGroupBox->setLayout(audioLayout);

      auto targetLabel = new QLabel(this);
      targetLabel->setText("Audio Source");
      audioLayout->addWidget(targetLabel);

      {
        auto targetLayout = new QHBoxLayout(this);
        audioLayout->addLayout(targetLayout);

        m_targetComboBox = new QComboBox(this);
        updateTargets();
        targetLayout->addWidget(m_targetComboBox);

        auto refreshTargetsButton = new QPushButton(this);
        refreshTargetsButton->setFixedSize(30, 30);
        refreshTargetsButton->setIcon(QIcon::fromTheme("view-refresh"));
        connect(refreshTargetsButton, &QPushButton::clicked, this,
                &StreamDialog::updateTargets);
        targetLayout->addWidget(refreshTargetsButton);
      }
    }

    auto button = new QPushButton(this);
    button->setText("Start Stream");
    connect(button, &QPushButton::clicked, this, &StreamDialog::startStream);
    layout->addWidget(button, Qt::AlignRight | Qt::AlignBottom);

    setLayout(layout);
  }

  setWindowTitle("discord-screenaudio Stream Dialog");
}

void StreamDialog::startStream() {
  auto resolution = m_resolutionComboBox->currentData().toString().split('x');
  emit requestedStreamStart(m_videoGroupBox->isChecked(),
                            m_audioGroupBox->isChecked(),
                            resolution[0].toUInt(), resolution[1].toUInt(),
                            m_framerateComboBox->currentData().toUInt(),
                            m_targetComboBox->currentText());
  setHidden(true);
}

void StreamDialog::updateTargets() {
  auto lastTarget = m_targetComboBox->currentText();

  m_targetComboBox->clear();
  m_targetComboBox->addItem("[All Desktop Audio]");
  for (auto target : Virtmic::getTargets()) {
    m_targetComboBox->addItem(target);
  }

  if (m_targetComboBox->findText(lastTarget) != -1)
    m_targetComboBox->setCurrentText(lastTarget);
  else
    m_targetComboBox->setCurrentText("[All Desktop Audio]");
}
