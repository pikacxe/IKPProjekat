#pragma once

/// <summary>
/// Subscribe to topic
/// </summary>
/// <param name="topic"></param>
void Subscribe(void* topic);

/// <summary>
/// Publish message to topic
/// </summary>
/// <param name="topic"></param>
/// <param name="message"></param>
void Publish(void* topic, void* message);

/// <summary>
/// Check if topic exists
/// </summary>
/// <param name="topic"></param>
/// <returns>True if exsists, otherwise false</returns>
bool TopicExists(void* topic);

/// <summary>
/// Get number of subscribers to topic
/// </summary>
/// <param name="topic"></param>
/// <returns>Number of subscribers</returns>
int NumberOfSubscribers(void* topic);