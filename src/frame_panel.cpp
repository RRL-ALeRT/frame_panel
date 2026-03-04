#include "frame_panel/frame_panel.hpp"
#include <pluginlib/class_list_macros.hpp>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

namespace my_frame_panel
{

FramePanel::FramePanel(QWidget* parent)
: Panel(parent)
{
  node_ = rclcpp::Node::make_shared("frame_panel_node");

  // Publishers
  pub_create_ = node_->create_publisher<std_msgs::msg::String>("/create_frame", 10);
  pub_delete_ = node_->create_publisher<std_msgs::msg::String>("/delete_frame", 10);
  pub_rename_ = node_->create_publisher<std_msgs::msg::String>("/rename_frame", 10);

  // Subscriber to keep the combo box updated
  sub_active_frames_ = node_->create_subscription<std_msgs::msg::String>(
    "/active_frames", 10,
    std::bind(&FramePanel::activeFramesCallback, this, std::placeholders::_1));

  // --- UI Setup ---
  QVBoxLayout* main_layout = new QVBoxLayout;

  frame_combo_ = new QComboBox;
  main_layout->addWidget(frame_combo_);

  name_edit_ = new QLineEdit;
  name_edit_->setPlaceholderText("Enter frame name...");
  main_layout->addWidget(name_edit_);

  reference_orientation_name = new QLineEdit;
  reference_orientation_name->setPlaceholderText("Enter parent frame name...");
  main_layout->addWidget(reference_orientation_name);

  QHBoxLayout* button_layout = new QHBoxLayout;
  add_button_ = new QPushButton("Add");
  remove_button_ = new QPushButton("Remove");
  rename_button_ = new QPushButton("Rename");

  button_layout->addWidget(add_button_);
  // Note: Skipping 'Duplicate' here for now, as we haven't implemented it yet!
  button_layout->addWidget(remove_button_);
  button_layout->addWidget(rename_button_);
  
  main_layout->addLayout(button_layout);
  setLayout(main_layout);

  // --- Connections ---
  connect(add_button_, &QPushButton::clicked, this, &FramePanel::onAddClicked);
  connect(remove_button_, &QPushButton::clicked, this, &FramePanel::onRemoveClicked);
  connect(rename_button_, &QPushButton::clicked, this, &FramePanel::onRenameClicked);
  
  qRegisterMetaType<QStringList>("QStringList");
  connect(this, &FramePanel::framesListUpdated, this, &FramePanel::updateComboBox);

  spin_timer_ = new QTimer(this);
  connect(spin_timer_, &QTimer::timeout, this, [this]() {
    rclcpp::spin_some(node_);
  });
  spin_timer_->start(50);
}

void FramePanel::onAddClicked()
{
  QString text = name_edit_->text();
  QString parent = reference_orientation_name->text();
  if (!text.isEmpty()) {
    std_msgs::msg::String msg;
    // Send name|reference_orientation_frame (reference is optional)
    if (!parent.isEmpty()) {
      msg.data = text.toStdString() + "|" + parent.toStdString();
    } else {
      msg.data = text.toStdString();
    }
    pub_create_->publish(msg);
    name_edit_->clear();
    reference_orientation_name->clear();
  }
}

void FramePanel::onRemoveClicked()
{
  QString current_frame = frame_combo_->currentText();
  if (!current_frame.isEmpty()) {
    std_msgs::msg::String msg;
    msg.data = current_frame.toStdString();
    pub_delete_->publish(msg);
  }
}

void FramePanel::onRenameClicked()
{
  QString current_frame = frame_combo_->currentText();
  QString new_text = name_edit_->text();
  
  if (!current_frame.isEmpty() && !new_text.isEmpty() && new_text != current_frame) {
    std_msgs::msg::String msg;
    // We send old_name|new_name to the server
    msg.data = current_frame.toStdString() + "|" + new_text.toStdString();
    pub_rename_->publish(msg);
    name_edit_->clear(); // Clear the box after renaming
  }
}

void FramePanel::activeFramesCallback(const std_msgs::msg::String::SharedPtr msg)
{
  QString data = QString::fromStdString(msg->data);
  QStringList frames = data.split(",", Qt::SkipEmptyParts);
  Q_EMIT framesListUpdated(frames);
}

void FramePanel::updateComboBox(const QStringList& frames)
{
  QString current_selection = frame_combo_->currentText();
  
  frame_combo_->blockSignals(true);
  frame_combo_->clear();
  frame_combo_->addItems(frames);
  
  int index = frame_combo_->findText(current_selection);
  if (index >= 0) {
    frame_combo_->setCurrentIndex(index);
  }
  frame_combo_->blockSignals(false);
}

}  // namespace my_frame_panel

PLUGINLIB_EXPORT_CLASS(my_frame_panel::FramePanel, rviz_common::Panel)