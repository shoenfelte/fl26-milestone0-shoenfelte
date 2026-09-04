# M0 Design and Understanding Note

Answer briefly in your own words. This is not intended to be a long report.

1. What responsibility belongs to `Workspace`, and what responsibilities belong to `Document`, `Prompt`, and `Message` instead?
Document, prompt, and message are all responsible for their own data and behavior only. Document knows its title, source path, and text, and can load itself from a file. Prompt knows a title and instruction text, and message knows a role and text. None of them know about another object or collections. On the other hand, workspace owns the std::vectors of each type, and is responsible for insertion order, counts, and indexed access.

2. Why are the collections inside `Workspace` private? Explain the purpose of the const and non-const `At` overloads.
The collections are private so Workspace controls how items are added and accessed. Callers can't resize, reorder, or replace the underlying containers directly; instead they have to go through addDocument/addPrompt/addMessage. This keeps insertion order fully under Workspace's control. The two At overloads exist so the same call syntax works whether the Workspace itself is mutable or const. The non-const overload returns a mutable reference so callers can edit a stored object in place without removing and re-adding it, while the const overload returns a const reference so a const Workspace& can still be read from without allowing modification.

3. Explain one meaningful test you added. What behavior does it check, and what implementation error could it catch?
The document_load_failure_preserves_state_test case constructs a Document with real title/contents, calls load() on a path that doesn't exist, and asserts that title(), sourcePath(), and contents() are all unchanged and that load() returned false. This checks the failure-state-preservation requirement mentioned in the specification. An implementation that opens the file, then  overwrites title_/sourcePath_/contents_ before checking whether the read actually succeeded would pass a "load a valid file" test but fail this one, so it catches a bug that would otherwise be missed.

4. Describe one implementation decision that you verified, tested, or revised before submitting your work.
I initially wrote add_test(NAME testworkspace COMMAND testworkspace) with no working directory set. Running the suite with ctest --test-dir build on its own  runs the test binary from inside build/, so Document::load("text/sample.txt") failed even though the same binary run directly as ./build/testworkspace from the project root passed. In that case the relative path resolved correctly. I ran both invocations from the Build Workflow section and comparing results, then fixed it by adding WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} to the add_test call so the relative sample path resolves the same way under both invocation methods.

5. If generative AI was used, disclose it as required by course policy. If no generative AI was used, state that. The disclosure itself is not used as proof of authorship or understanding.
Generative AI (Claude) was used to identify and fix the error mentioned in the previous question.
