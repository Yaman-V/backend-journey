package com.taskmanager.api.model;

import java.util.Objects;
import java.util.UUID;
import java.util.concurrent.atomic.AtomicLong;

public class Task {

    public enum Status {
        TODO,
        IN_PROGRESS,
        DONE,
        SUSPENDED
    }
    private static final AtomicLong ID_COUNTER = new AtomicLong(1);

    private final Long id;
    private String title;
    private String description;
    private Status status;

    public Task( String title, String description , Status status) {
        this.id = ID_COUNTER.getAndIncrement();
        this.title = Objects.requireNonNull(title, "title must not be null");
        this.description = (description != null)? description : "";
        this.status = (status != null)? status : Status.TODO;
    }

    public Long getId() {
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