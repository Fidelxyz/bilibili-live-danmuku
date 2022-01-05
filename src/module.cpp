#include "module.h"

const ModuleMetadata Module::moduleMetadata = {"default_module_name", {}};

Module::Module(QWidget *parent) : QObject((QObject *)parent) {
    widget = new QWidget;
    // TODO check dependency
}

Module::~Module() { delete widget; }

QWidget *Module::getWidget() const { return widget; }
