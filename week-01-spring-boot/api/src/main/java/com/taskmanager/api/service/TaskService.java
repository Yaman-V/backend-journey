package com.taskmanager.api.service;

import com.taskmanager.api.model.Task;
import org.springframework.stereotype.Service;

import java.util.ArrayList;
import java.util.List;
import java.util.UUID;

@Service
public class TaskService {
    private List<Task> tasksList = new ArrayList<>();

    public TaskService() {
        tasksList.add(new Task(UUID.randomUUID(), "Master Spring Boot-1", "Learn annotations and MVC architecture", Task.Status.DONE));
        tasksList.add(new Task(UUID.randomUUID(), "Master Spring Boot-2", "Learn annotations and MVC architecture", Task.Status.IN_PROGRESS));
        tasksList.add(new Task(UUID.randomUUID(), "Review", "Review what I have learned", Task.Status.TODO));
        tasksList.add(new Task(UUID.randomUUID(), "Submit", "Send the project to friends", Task.Status.SUSPENDED));
    }

    public List<Task> getAllTask(){
        return tasksList;
    }
}
