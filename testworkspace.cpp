#include <cassert>
#include <iostream>
#include <stdexcept>

#include "Prompt.hpp"
#include "Workspace.hpp"
#include "Document.hpp"
#include "Message.hpp"

void prompt_smoke_test() {
    Prompt p("Reviewer", "Review this requirement.");
    assert(p.title() == "Reviewer");
    assert(!p.empty());
}

void workspace_smoke_test() {
    Workspace w("Demo");
    w.addPrompt(Prompt("Reviewer", "Review this requirement."));
    assert(w.promptCount() == 1);
}


// document
void document_default_construction_test() {
    Document d;
    assert(d.title().empty());
    assert(d.sourcePath().empty());
    assert(d.contents().empty());
    assert(d.empty());
}

void document_value_construction_test() {
    Document d("My Title", "Some contents");
    assert(d.title() == "My Title");
    assert(d.sourcePath().empty());
    assert(d.contents() == "Some contents");
    assert(!d.empty());
    assert(d.characterCount() == d.contents().size());
}

void document_equality_test() {
    Document a("T", "C");
    Document b("T", "C");
    Document c("T", "different");
    assert(a == b);
    assert(a != c);
}

void document_load_success_test() {
    Document d;
    bool ok = d.load("text/sample.txt");
    assert(ok);
    assert(d.sourcePath() == "text/sample.txt");
    assert(d.title() == "sample.txt");
    assert(!d.contents().empty());
}

void document_load_failure_preserves_state_test() {  // bouundary case
    Document d("Original Title", "Original contents");
    bool ok = d.load("text/does_not_exist.txt");
    assert(!ok);
    assert(d.title() == "Original Title");
    assert(d.sourcePath().empty());
    assert(d.contents() == "Original contents");
}

// prompt
// void prompt_smoke_test() {
void prompt_default_and_empty_test() {
    Prompt p;
    assert(p.title().empty());
    assert(p.text().empty());
    assert(p.empty());

    p.setText("not empty");
    assert(!p.empty());
}

void prompt_equality_test() {
    Prompt a("T", "X");
    Prompt b("T", "X");
    Prompt c("T", "Y");
    assert(a == b);
    assert(a != c);
}

// message
void message_default_and_roles_test() {
    Message m;
    assert(m.role() == MessageRole::User);
    assert(m.empty());

    Message sys(MessageRole::System, "You are a helpful assistant.");
    assert(sys.role() == MessageRole::System);
    assert(!sys.empty());

    sys.setRole(MessageRole::Assistant);
    assert(sys.role() == MessageRole::Assistant);
}

void message_equality_test() {
    Message a(MessageRole::User, "hi");
    Message b(MessageRole::User, "hi");
    Message c(MessageRole::Assistant, "hi");
    assert(a == b);
    assert(a != c); // same text, different role must not be equal
}

// workspace
// void workspace_smoke_test() {
void workspace_insertion_order_and_counts_test() {
    Workspace w("Demo");
    w.addDocument(Document("First", "1"));
    w.addDocument(Document("Second", "2"));
    w.addMessage(Message(MessageRole::User, "hello"));

    assert(w.documentCount() == 2);
    assert(w.promptCount() == 0);
    assert(w.messageCount() == 1);
    assert(w.documentAt(0).title() == "First");
    assert(w.documentAt(1).title() == "Second");
}

void workspace_invalid_index_throws_test() {  // boundary case
    Workspace w("Demo");
    w.addDocument(Document("Only", "content"));

    bool threw = false;
    try {
        w.documentAt(5);
    } catch (const std::out_of_range&) {
        threw = true;
    }
    assert(threw);
}

void workspace_mutable_at_modifies_stored_object_test() {
    Workspace w("Demo");
    w.addPrompt(Prompt("Original", "text"));
    w.promptAt(0).setTitle("Renamed");
    assert(w.promptAt(0).title() == "Renamed");
}

void workspace_equality_test() {
    Workspace a("Demo");
    Workspace b("Demo");
    assert(a == b);

    a.addMessage(Message(MessageRole::User, "hi"));
    assert(a != b);

    b.addMessage(Message(MessageRole::User, "hi"));
    assert(a == b);
}

void workspace_copy_independence_test() {
    Workspace original("Demo");
    original.addDocument(Document("Doc1", "content"));

    Workspace copy = original;
    assert(copy == original);

    original.addDocument(Document("Doc2", "more content"));
    original.setName("Changed");

    assert(copy.documentCount() == 1);
    assert(copy.name() == "Demo");
    assert(original.documentCount() == 2);
    assert(original != copy);
}



/////////////
int main() {
    prompt_smoke_test();
    workspace_smoke_test();

    // TODO: Add your own meaningful M0 tests.
    document_default_construction_test();
    document_value_construction_test();
    document_equality_test();
    document_load_success_test();
    document_load_failure_preserves_state_test();

    prompt_default_and_empty_test();
    prompt_equality_test();

    message_default_and_roles_test();
    message_equality_test();

    workspace_insertion_order_and_counts_test();
    workspace_invalid_index_throws_test();
    workspace_mutable_at_modifies_stored_object_test();
    workspace_equality_test();
    workspace_copy_independence_test();
    
    std::cout << "M0 tests passed\n";
    return 0;
}
