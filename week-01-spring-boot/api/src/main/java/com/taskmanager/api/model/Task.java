package com.taskmanager.api.model;

import java.util.Objects;
import java.util.UUID;

public class Task {

    public enum Status {
        TODO,
        IN_PROGRESS,
        DONE,
        SUSPENDED
    }

    private final UUID id;
    private String title;
    private String description;
    private Status status;

    public Task(UUID id, String title, String description, Status status) {
        this.id = Objects.requireNonNull(id, "id must not be null");
        this.title = Objects.requireNonNull(title, "title must not be null");
        this.description = description;
        this.status = Objects.requireNonNull(status, "status must not be null");
    }

    public UUID getId() {
        return id;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = Objects.requireNonNull(title, "title must not be null");
    }

    public String getDescription() {
        return description;
    }

    public void setDescription(String description) {
        this.description = description;
    }

    public Status getStatus() {
        return status;
    }

    public void setStatus(Status status) {
        this.status = Objects.requireNonNull(status, "status must not be null");
    }
}